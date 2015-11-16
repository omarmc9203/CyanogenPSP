#include "settingsMenu.h"
#include "language.h"
#include "include/pgeZip.h"
#include "include/ram.h"
#include "include/utils.h"
#include "include/pspusbdevice.h"
#include "include/common.h"
#include "homeMenu.h"
#include "appDrawer.h"
#include "fileManager.h"
#include "clock.h"
#include "gameLauncher.h"
#include "gallery.h"
#include "lockScreen.h"
#include "recentsMenu.h"
#include "powerMenu.h"
#include "recoveryMenu.h"
#include "screenshot.h"

char name;
int setclock;
char cyanogenpspversion[5] = "5.3";
char lang[12] = "Uk English";
static char Settings_message[100] = "";

char buffer[100] = "";

//kernel function imports
int imposeSetBrightness(int value);

int connectAPCallback(int state) //Internet stuff
{
    oslStartDrawing();
    oslDrawImageXY(wifibg, 0, 19);
    oslDrawStringf(30, 175, "Connecting to AP...");
    sprintf(buffer, "State: %i", state);
    oslDrawStringf(30, 195, buffer);
    oslEndDrawing();
    oslEndFrame();
    oslSyncFrame();

    return 0;
} 
 
int connectToAP(int config) //Internet stuff
{
    oslStartDrawing();
    oslDrawImageXY(wifibg, 0, 19);
    oslDrawStringf(30, 175, "Connecting to AP...");
    oslEndDrawing();
    oslEndFrame();
    oslSyncFrame();

    int result = oslConnectToAP(config, 30, connectAPCallback);
    if (!result){
        char ip[30] = "";
        char resolvedIP[30] = "";

        oslStartDrawing();
        oslDrawImageXY(wifibg, 0, 19);
        oslGetIPaddress(ip);
        sprintf(buffer, "IP address: %s", ip);
        oslDrawStringf(30, 175, buffer);

        sprintf(buffer, "Resolving %s", Address);
        oslDrawStringf(30, 195, buffer);
        oslEndDrawing();
        oslEndFrame();
        oslSyncFrame();

        result = oslResolveAddress(Address, resolvedIP);

        oslStartDrawing();
        oslDrawImageXY(wifibg, 0, 19);
        oslGetIPaddress(ip);
        if (!result)
            sprintf(buffer, "Resolved IP address: %s", ip);
        else
            sprintf(buffer, "Error resolving address!");
        oslDrawStringf(30, 195, buffer);
        oslEndDrawing();
        oslEndFrame();
        oslSyncFrame();
		sceKernelDelayThread(3*1000000);
    }else{
        oslStartDrawing();
        oslDrawImageXY(wifibg, 0, 19);
        sprintf(buffer, "Error connecting to AP!");
        oslDrawStringf(30, 195, buffer);
        oslEndDrawing();
        oslEndFrame();
        oslSyncFrame();
		sceKernelDelayThread(3*1000000);
    }
    oslDisconnectFromAP();
    return 0;
} 

void onlineUpdater()
{
	int skip = 0;
    int browser = 0;
	char message[100] = "";
	
	oslNetInit();

    while(!osl_quit)
	{
        browser = oslBrowserIsActive();
		if (!skip)
		{
            oslStartDrawing();
			
            if (browser)
			{
                oslDrawBrowser();
                if (oslGetBrowserStatus() == PSP_UTILITY_DIALOG_NONE)
				{
                    oslEndBrowser();
					updateReady = 1;
					aboutMenu();
                }
            }
            oslEndDrawing();
		}
		oslEndFrame();
		skip = oslSyncFrame();

        if (!browser)
		{
            oslReadKeys();
            oslBrowserInit("http://downloads.sourceforge.net/project/cyanogenpsp/Updates/update.zip", "/PSP/GAME", 14*1024*1024,
                                         PSP_UTILITY_HTMLVIEWER_DISPLAYMODE_SMART_FIT,
                                         PSP_UTILITY_HTMLVIEWER_DISABLE_STARTUP_LIMITS,
                                         PSP_UTILITY_HTMLVIEWER_INTERFACEMODE_FULL,
                                         PSP_UTILITY_HTMLVIEWER_CONNECTMODE_MANUAL_ALL);
			memset(message, 0, sizeof(message));

        }
		
		captureScreenshot();
    }
	oslNetTerm();
}

void flashUpdate()
{
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!recoverybg)
		debugDisplay();
	
	while (!osl_quit)
	{	
		oslStartDrawing();
		oslClearScreen(RGB(0,0,0));
		oslDrawImageXY(recoverybg, 0, 0);
		oslDrawStringf(10,60,"Flashing zip...");
		if (fileExists("ms0:/PSP/GAME/update.zip"))
		{		
			pgeZip* zipFiles = pgeZipOpen("../update.zip");
			chdir("..");
			pgeZipExtract(zipFiles, NULL);
			pgeZipClose(zipFiles);
			oslIntraFontSetStyle(Roboto, 0.5f, WHITE,0,INTRAFONT_ALIGN_LEFT);
			oslDrawStringf(10,80,"Installed Successfully.");
			oslDrawStringf(10,90,"Exiting..");
			oslSyncFrame();
			sceKernelDelayThread(2*1000000);
			oslSyncFrame();
			oslDrawStringf(10,50,"Enjoy :)");
			sceKernelDelayThread(3*1000000);
			oslDeleteImage(recoverybg);
			sceIoRemove("ms0:/PSP/GAME/CyanogenPSP/system/build.prop");
			sceKernelExitGame();
		}
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();
	}
}
	
void switchStatus(int n)
{
	if (n == 1)
	{
		if (sceWlanGetSwitchState() == 0)
			oslDrawImageXY(offswitch, 370, 62);
	
		else
			oslDrawImageXY(onswitch, 370, 62);
	}
	
	else if (n == 2)
	{
		if (sceWlanGetSwitchState() == 0)
		{
			oslDrawImageXY(offswitch, 392, 68);
		}
		
		else
		{
			oslDrawImageXY(onswitch, 392, 68);
		}
	}
	
	else if (n == 3)
	{
		if (sceWlanGetSwitchState() == 0)
		{
			oslDrawImageXY(offswitch, 392, 68);
			oslDrawStringf(58,74,"Off");
		}
		
		else
		{
			oslDrawImageXY(onswitch, 392, 68);
			oslDrawStringf(58,74,"On");
		}
	}
}

int getCpuClock()
{
    return scePowerGetCpuClockFrequency();
}

int getBusClock()
{
    return scePowerGetBusClockFrequency();
}

void pspGetModel(int x, int y)
{
	int pspmodel = kuKernelGetModel();
	
	switch(pspmodel)
	{
		case 0:
			oslDrawStringf(x,y,"Model: PSP 1000");
			break;
   
		case 1:
			oslDrawStringf(x,y,"Model: PSP 2000");
			break;
   
		case 2:
			oslDrawStringf(x,y,"Model: PSP 3000");
			break;
   
		case 3:
			oslDrawStringf(x,y,"Model: PSP 3000");
			break;
		
		case 4:
			oslDrawStringf(x,y,"Model: PSP Go N1000");
			break;
   
		default:
			oslDrawStringf(x,y,"Model: PS Vita");
			break;
	}
}

fw_version getFwVersion(fw_version *v);

void aboutMenu()
{	
	aboutbg = oslLoadImageFilePNG(aboutBgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	if (!aboutbg || !highlight)
		debugDisplay();

	u8 *macAddress = oslAdhocGetMacAddress();
	
	fw_version version;
	getFwVersion(&version);
	
	char message[100] = "";
    int dialog = OSL_DIALOG_NONE;
	
	int updateActivation = 1;

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(aboutbg, 0, 0);
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);

		oslDrawStringf(20,78,"%s", lang_settingsAbout[language][0]);
		oslDrawStringf(20,92,"%s", lang_settingsAbout[language][1]);
		pspGetModel(20,143);
		oslDrawStringf(20,129,"%s %s-20151023-%s", lang_settingsAbout[language][2], cyanogenpspversion, lang_settingsAbout[language][3]);
		oslDrawStringf(20,157,"%s %02X:%02X:%02X:%02X:%02X:%02X", lang_settingsAbout[language][4], macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
		oslDrawStringf(20,185,"%s %d.%d", lang_settingsAbout[language][5], version.major, version.minor);
		oslDrawStringf(20,199,"%s %s", lang_settingsAbout[language][6], OSL_VERSION);
		oslDrawStringf(20,213,"Developer: Joel16");

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 62 && cursor->y <= 119)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,78,"%s", lang_settingsAbout[language][0]);
			oslDrawStringf(20,92,"%s", lang_settingsAbout[language][1]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(aboutbg);
				oslDeleteImage(highlight);
				updatesMenu();
			}
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 120 && cursor->y <= 178)
		{
			oslDrawImageXY(highlight, 0, 122);
			pspGetModel(20,143);
			oslDrawStringf(20,129,"%s %s-20151023-%s", lang_settingsAbout[language][2], cyanogenpspversion, lang_settingsAbout[language][3]);
			oslDrawStringf(20,157,"%s %02X:%02X:%02X:%02X:%02X:%02X", lang_settingsAbout[language][4], macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		dialog = oslGetDialogType();
        if (dialog)
		{
			oslDrawDialog();
            if (oslGetDialogStatus() == PSP_UTILITY_DIALOG_NONE)
			{
				if (oslDialogGetResult() == OSL_DIALOG_CANCEL)
					sprintf(message, "Cancel");
				else if (dialog == OSL_DIALOG_MESSAGE)
				{
					int button = oslGetDialogButtonPressed();
					if (button == PSP_UTILITY_MSGDIALOG_RESULT_YES)
						flashUpdate();
					else if (button == PSP_UTILITY_MSGDIALOG_RESULT_NO)
						updateActivation = 0;
						oslEndDialog();
				}
            oslEndDialog();
            }
		}
		
		if (dialog == OSL_DIALOG_NONE)
		{
			if (updateReady == 1)
			{
				if (updateActivation == 1)
				{
					oslInitMessageDialog("Update.zip has been found. Would you like to flash the update?", 1);
					memset(message, 0, sizeof(message));
				}
			}
		}
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{	
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			settingsMenu();	
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			settingsMenu();	
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			home();	
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void updatesMenu()
{		    
	updatesbg = oslLoadImageFilePNG(updatesBgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);

	if (!updatesbg || !highlight)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(updatesbg, 0, 0);
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);
		
		oslDrawStringf(20,93, "%s", lang_settingsUpdates[language][0]);

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 63 && cursor->y <= 113)
		{
			oslDrawImageXY(highlight, 0, 70);
			oslDrawStringf(20,93, "%s", lang_settingsUpdates[language][0]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				onlineUpdater();
			}
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(updatesbg);
			oslDeleteImage(highlight);
			aboutMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(updatesbg);
			oslDeleteImage(highlight);
			aboutMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(updatesbg);
			oslDeleteImage(highlight);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
	oslNetTerm();
}

void performanceMenu()
{	
	performancebg = oslLoadImageFilePNG(performanceBgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);

	if (!performancebg || !highlight)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(performancebg, 0, 0);
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);

		oslDrawStringf(20,86, "%s", lang_settingsPerformance[language][0]);
		oslDrawStringf(20,140, "%s", lang_settingsPerformance[language][1]);
		oslDrawStringf(20,194, "%s", lang_settingsPerformance[language][2]);
		oslDrawStringf(20,248, "%s", lang_settingsPerformance[language][3]);

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,86, "%s", lang_settingsPerformance[language][0]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(highlight);
				oslDeleteImage(performancebg);
				processorMenu();
			}
		}

		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,140, "%s", lang_settingsPerformance[language][1]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(highlight);
				oslDeleteImage(performancebg);
				ramMenu();
			}
		}

		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawStringf(20,194, "%s", lang_settingsPerformance[language][2]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(highlight);
				oslDeleteImage(performancebg);
				storageMenu();
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 216 && cursor->y <= 272)
		{
			oslDrawImageXY(highlight, 0, 227);
			oslDrawStringf(20,248, "%s", lang_settingsPerformance[language][3]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(highlight);
				oslDeleteImage(performancebg);
				batteryMenu();
			}
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			settingsMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void setCpuBoot()
{	
	processorState = setFileDefaultsInt("system/settings/processorInfo.bin", 4, processorState);
	
	switch(processorState)
	{
		case 0:
			scePowerSetClockFrequency(20, 20, 10);
			break;
		
		case 1:
			scePowerSetClockFrequency(75, 75, 37);
			break;
			
		case 2:
			scePowerSetClockFrequency(100, 100, 50);
			break;
		
		case 3:
			scePowerSetClockFrequency(133, 133, 66);
			break;
		
		case 4:
			scePowerSetClockFrequency(222, 222, 111);
			break;
		
		case 5:
			scePowerSetClockFrequency(266, 266, 133);
			break;
		
		case 6:
			scePowerSetClockFrequency(300, 300, 150);
			break;
		
		case 7:
			scePowerSetClockFrequency(333, 333, 166);
			break;
	}
}

void processorMenu()
{	
	FILE * processorInfo;
	
	int currentState = stateOff;
	int cpufreq, cpu, bus, state = 0;
	
	processorbg = oslLoadImageFilePNG(performanceBgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	if (!processorbg || !highlight)
		debugDisplay(); 
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslDrawImageXY(processorbg, 0, 0);
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);
		
		oslDrawStringf(20,76, "%s", lang_settingsProcessor[language][0]);
		oslDrawStringf(20,128, "%s", lang_settingsProcessor[language][1]);
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 118 && cursor->y <= 174)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,128, "%s", lang_settingsProcessor[language][1]);
			oslDrawStringf(20,145, "%s", lang_settingsProcessor[language][2]);
		}
		
		if (osl_keys->pressed.L && batteryM != 0)
		{
			currentState = stateUnderClock;
		}
		else if (osl_keys->pressed.R && batteryM != 0)
		{
			currentState = stateOverClock;
		}
		
		if (currentState == stateOverClock)
		{
			processorInfo = fopen("system/settings/processorInfo.bin", "w");
			cpufreq = scePowerGetCpuClockFrequency();
			
			if (cpufreq <70)
			{
				state = 1;
		        scePowerSetClockFrequency(75, 75, 37);	
			}
			else if (cpufreq <95)
			{
				state = 2;
				scePowerSetClockFrequency(100, 100, 50);
			}
			else if (cpufreq <125)
			{
				state = 3;
				scePowerSetClockFrequency(133, 133, 66);
			}
			else if (cpufreq <215)
			{
				state = 4;
				scePowerSetClockFrequency(222, 222, 111);
			}
			else if (cpufreq <260)
			{
				state = 5;
				scePowerSetClockFrequency(266, 266, 133);
			}
			else if (cpufreq <290)
			{
				state = 6;
				scePowerSetClockFrequency(300, 300, 150);
			}
			else if (cpufreq <325)
			{
				state = 7;
				scePowerSetClockFrequency(333, 333, 166);
			}
			
			fprintf(processorInfo, "%d", state);
			fclose(processorInfo);
			currentState = stateNoClock;
		}
		
		if (currentState == stateUnderClock)
		{
			processorInfo = fopen("system/settings/processorInfo.bin", "w");
			cpufreq = scePowerGetCpuClockFrequency();
			
			if (cpufreq> 300)
			{
				state = 6;
				scePowerSetClockFrequency(300, 300, 150);
			}
			else if (cpufreq> 266)
			{
				state = 5;
				scePowerSetClockFrequency(266, 266, 133);
			}
			else if (cpufreq> 222)
			{
				state = 4;
				scePowerSetClockFrequency(222, 222, 111);
			}
			else if (cpufreq> 133)
			{
				state = 3;
				scePowerSetClockFrequency(133, 133, 66);
			}
			else if (cpufreq> 100)
			{
				state = 2;
				scePowerSetClockFrequency(100, 100, 50);
			}
			else if (cpufreq> 75)
		    {
				state = 1;
				scePowerSetClockFrequency(75, 75, 37);
			}
			else if (cpufreq> 20)
			{
				state = 0;
				scePowerSetClockFrequency(20, 20, 10);
			}
			
			fprintf(processorInfo, "%d", state);
			fclose(processorInfo);
			currentState = stateNoClock; // This state allows the frequency to not climb on each cycle.
		}
		
		cpu = getCpuClock(); 
		bus = getBusClock(); 
		
		oslDrawStringf(20,87,"%d/%d",cpu, bus);
		oslDrawStringf(20,189, "%s", lang_settingsProcessor[language][0]);
		oslDrawStringf(20,202,"%d MHz", cpu);
		oslDrawStringf(20,241, "%s", lang_settingsProcessor[language][3]);
		oslDrawStringf(20,254,"%d MHz", bus);
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(processorbg);
			oslDeleteImage(highlight);
			performanceMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(processorbg);
			oslDeleteImage(highlight);
			performanceMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(processorbg);
			oslDeleteImage(highlight);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void ramMenu()
{	
	performancebg = oslLoadImageFilePNG(performance2BgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	if (!performancebg || !highlight)
		debugDisplay();
	
	OSL_MEMSTATUS ram = oslGetRamStatus();
	int availableRam = (ram.maxAvailable/1000000);
	int totalRam = ((20*1024*1024)/1000000);
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslDrawImageXY(performancebg, 0, 0);
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);
		
		oslDrawStringf(20,78,"%s %d MB (%d%%) %s\n", lang_settingsRAM[language][0], availableRam, (((availableRam) * 100)/(totalRam)), lang_settingsRAM[language][1]); 
		
		oslDrawFillRect(20, 100, 424, 110, RGB(206, 215, 219));
		oslDrawFillRect(20, 100, ((availableRam)*20.2), 110, RGB(0, 149, 135));
		
		oslDrawStringf(20,122,"%d%% %s", (((20 - (availableRam)) * 100)/(totalRam)), lang_settingsRAM[language][2]);
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			performanceMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			performanceMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void storageMenu()
{	
	performancebg = oslLoadImageFilePNG(performance2BgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	unsigned int buf[5]; 
	unsigned int *pbuf = buf; 
	sceIoDevctl("ms0:", 0x02425818, &pbuf, sizeof(pbuf), 0, 0);
	
	double freeSpace = buf[1]*buf[3]*buf[4];
	//double totalSpace = buf[0]*buf[3]*buf[4];
	
	if (!performancebg || !highlight)
		debugDisplay();
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslDrawImageXY(performancebg, 0, 0);
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);
		
		oslDrawStringf(20,80, "%s", lang_settingsStorage[language][0]); 
		//oslDrawStringf(20,100,"%s %.2f MB", lang_settingsStorage[language][1], totalSpace/1048576);
		oslDrawStringf(20,120,"%s %.2f MB", lang_settingsStorage[language][2], freeSpace/1048576);
		
		if (osl_keys->pressed.select)
		{
			enableUsb();
		}
		
		else if (osl_keys->pressed.select)
		{
			disableUsb();
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			performanceMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			performanceMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void batteryMenu()
{	
	FILE * batteryManagement;

	performancebg = oslLoadImageFilePNG(performanceBgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	select = oslLoadImageFilePNG("system/settings/select.png", OSL_IN_RAM, OSL_PF_8888);
	deselect = oslLoadImageFilePNG("system/settings/deselect.png", OSL_IN_RAM, OSL_PF_8888);
	
	batteryM = setFileDefaultsInt("system/settings/battery.bin", 1, batteryM);
	
	oslSetFont(Roboto);

	if (!performancebg || !highlight)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(performancebg, 0, 0);
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);

		oslDrawStringf(20,86, "%s", lang_settingsBattery[language][0]);
		oslDrawStringf(20,140, "%s", lang_settingsBattery[language][1]);
		oslDrawStringf(20,194, "%s", lang_settingsBattery[language][2]);

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,86, "%s", lang_settingsBattery[language][0]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);
				batteryManagement = fopen("system/settings/battery.bin", "w"); 
				fprintf(batteryManagement, "0");
				fclose(batteryManagement);
				batteryM = setFileDefaultsInt("system/settings/battery.bin", 1, batteryM);
				setPowerManagement();
			}
		}

		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,140, "%s", lang_settingsBattery[language][1]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);
				batteryManagement = fopen("system/settings/battery.bin", "w"); 
				fprintf(batteryManagement, "1");
				fclose(batteryManagement);
				batteryM = setFileDefaultsInt("system/settings/battery.bin", 1, batteryM);
				setPowerManagement();
			}
		}

		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawStringf(20,194, "%s", lang_settingsBattery[language][2]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);
				batteryManagement = fopen("system/settings/battery.bin", "w"); 
				fprintf(batteryManagement, "2");
				fclose(batteryManagement);
				batteryM = setFileDefaultsInt("system/settings/battery.bin", 1, batteryM);
				setPowerManagement();
			}
		}
		
		switch(batteryM)
		{
			case 0:
				oslDrawImageXY(select, 400, 80);
				oslDrawImageXY(deselect, 400, 135);
				oslDrawImageXY(deselect, 400, 190);
				break;
	
			case 1:
				oslDrawImageXY(select, 400, 135);
				oslDrawImageXY(deselect, 400, 80);
				oslDrawImageXY(deselect, 400, 190);
				break;
			
			case 2:
				oslDrawImageXY(select, 400, 190);
				oslDrawImageXY(deselect, 400, 80);
				oslDrawImageXY(deselect, 400, 135);
				break;
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			oslDeleteImage(select);
			oslDeleteImage(deselect);
			performanceMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			oslDeleteImage(select);
			oslDeleteImage(deselect);
			performanceMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			oslDeleteImage(select);
			oslDeleteImage(deselect);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void setPowerManagement()
{
	FILE * processorInfo = fopen("system/settings/processorInfo.bin", "w");
	
	switch(batteryM)
	{
		case 0:
			processorState = 4;
			fprintf(processorInfo, "%d", processorState);
			setCpuBoot();
			imposeSetBrightness(0);
			break;
	
		case 1:
			processorState = 4;
			fprintf(processorInfo, "%d", processorState);
			setCpuBoot();
			imposeSetBrightness(1);
			break;
			
		case 2:
			processorState = 7;
			fprintf(processorInfo, "%d", processorState);
			setCpuBoot();
			break;
	}
	
	fclose(processorInfo);
}

void displayMenu()
{	
	displaybg = oslLoadImageFilePNG(displayBgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);

	if (!displaybg)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);

		oslDrawImageXY(displaybg, 0, 0);
		
		oslDrawStringf(20,86, "%s", lang_settingsDisplay[language][0]);
		oslDrawStringf(20,140, "%s", lang_settingsDisplay[language][1]);
		oslDrawStringf(20,195, "%s", lang_settingsDisplay[language][2]);
		oslDrawStringf(20,245, "%s", lang_settingsDisplay[language][3]);
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,86, "%s", lang_settingsDisplay[language][0]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displayThemes();
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,140, "%s", lang_settingsDisplay[language][1]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displayTime();
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawStringf(20,195, "%s", lang_settingsDisplay[language][2]);
			if (osl_keys->pressed.cross)
			{	
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displaySubThemes("system/settings/language", 4);
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 216 && cursor->y <= 272)
		{
			oslDrawImageXY(highlight, 0, 228);
			oslDrawStringf(20,245, "%s", lang_settingsDisplay[language][3]);
			if (osl_keys->pressed.cross)
			{	
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displayMiscellaneous();
			}
		}

		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			settingsMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void displayThemes()
{	
	displaybg = oslLoadImageFilePNG(displayBgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);

	if (!displaybg)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);

		oslDrawImageXY(displaybg, 0, 0);
		
		oslDrawStringf(20,86, "%s", lang_settingsThemes[language][0]);
		oslDrawStringf(20,140, "%s", lang_settingsThemes[language][1]);
		oslDrawStringf(20,194, "%s", lang_settingsThemes[language][2]);
		oslDrawStringf(20,248, "%s", lang_settingsThemes[language][3]);
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,86, "%s", lang_settingsThemes[language][0]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displaySubThemes("system/themes", 3);
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,140, "%s", lang_settingsThemes[language][1]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displaySubThemes("system/framework/framework-res/res", 1);
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawStringf(20,194, "%s", lang_settingsThemes[language][2]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displaySubThemes("system/fonts", 0);
			}
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 216 && cursor->y <= 272)
		{
			oslDrawImageXY(highlight, 0, 227);
			oslDrawStringf(20,248, "%s", lang_settingsThemes[language][3]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displaySubThemes("system/icons", 2);
			}
		}

		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			displayMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			displayMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void settingsDisplay()
{	
	oslDrawImageXY(displaybg, 0, 0);
	oslDrawImageXY(highlight,0,(current - curScroll)*56+SETTINGS_CURR_DISPLAY_Y);
	
	oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
	battery(330,2,0);
	digitaltime(381,4,0,hrTime);
	
	// Displays the directories, while also incorporating the scrolling
	for(i=curScroll;i<MAX_SETTINGS_DISPLAY+curScroll;i++) 
	{
		// Handles the cursor and the display to not move past 4.
		// For moving down
		//if ((folderIcons[i].active == 0) && (current >= i-1)) {
	
		if ((folderIcons[i].active == 0) && (current >= i-1)) 
		{
			current = i-1;
			break;
		}
		// For moving up
		if (current <= curScroll-1) 
		{
			current = curScroll-1;
			break;
		}
		
		// If the currently selected item is active, then display the name
		if (folderIcons[i].active == 1) 
		{
			oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);
			oslDrawStringf(SETTINGS_DISPLAY_X, (i - curScroll)*55+SETTINGS_DISPLAY_Y, "%.56s", folderIcons[i].name);	// change the X & Y value accordingly if you want to move it (for Y, just change the +10)		
		}
	}
}

void settingsUp()
{
	current--; // Subtract a value from current so the ">" goes up
	if ((current <= curScroll-1) && (curScroll > 1)) {
		curScroll--; // To do with how it scrolls
	}
}

void settingsDown()
{
	if (folderIcons[current+1].active) current++; // Add a value onto current so the ">" goes down
	if (current >= (MAX_SETTINGS_DISPLAY+curScroll)) {
		curScroll++; // To do with how it scrolls
	}
}

void settingsUpx5()
{
	current -= 5;  // Subtract a value from current so the ">" goes up
	if ((current <= curScroll-1) && (curScroll > 1)) {
		curScroll -= 5;  // To do with how it scrolls
	}
}

void settingsDownx5()
{
	if (folderIcons[current+1].active) current += 5; // Add a value onto current so the ">" goes down
	if (current >= (MAX_SETTINGS_DISPLAY+curScroll)) {
		curScroll += 5; // To do with how it scrolls
	}
}

void changeFont() //Created a separated function for this only because deleting a font while its in use causes it to crash.
{
	while (!osl_quit)
	{		
		oslDeleteFont(Roboto);
		oslPlaySound(KeypressStandard, 1);  
		FILE * fontPathTXT = fopen("system/settings/font.bin", "w");
		fprintf(fontPathTXT,"%s", folderIcons[current].filePath);
		fclose(fontPathTXT);
		Roboto = oslLoadIntraFontFile(folderIcons[current].filePath, INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
		oslSetFont(Roboto);
		return;
	}
}

int changeLanguage() //Created a separated function for this only because deleting a font while its in use causes it to crash.
{
	if (strcmp(folderIcons[current].filePath, "system/settings/language/English") == 0)
		language = 0;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/French") == 0)
		language = 1;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Polish") == 0)
		language = 2;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/German") == 0)
		language = 3;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Dutch") == 0)
		language = 4;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Spanish") == 0)
		language = 5;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Portuguese") == 0)
		language = 6;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Brazilian-Portuguese") == 0)
		language = 7;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Norwegian") == 0)
		language = 8;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Romanian") == 0)
		language = 9;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Italian") == 0)
		language = 10;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Russian") == 0)
		language = 11;
	else if (strcmp(folderIcons[current].filePath, "system/settings/language/Ukrainian") == 0)
		language = 12;
	
	FILE * languagePath = fopen("system/settings/language.bin", "w");
	fprintf(languagePath, "%d", language);
	fclose(languagePath);
	
	return language;
}

void themesLoad()
{	
	strcpy(themeDirPath, setFileDefaultsChar("system/settings/theme.bin", "system", themeDirPath));

	char settingsBgImg[100] = "/settings/settingsbg.png";
	char displayBgImg[100] = "/settings/displaybg.png";
	char aboutBgImg[100] = "/settings/aboutbg.png";
	char developerBgImg[100] = "/settings/developerbg.png";
	char performanceBgImg[100] = "/settings/performancebg.png";
	char performance2BgImg[100] = "/settings/performancebg2.png";
	char securityBgImg[100] = "/settings/securitybg.png";
	char updatesBgImg[100] = "/settings/updatesbg.png";
	char wifiBgImg[100] = "/settings/wifibg.png";
	char offSwitchImg[100] = "/settings/offswitch.png";
	char onSwitchImg[100] = "/settings/onswitch.png";
	
	char apolloBgImg[100] = "/app/apollo/mp3bg.png";
	char nowplayingBgImg[100] = "/app/apollo/nowplaying.png";
	
	char fmBgImg[100] = "/app/filemanager/filemanagerbg.png";
	char diriconImg[100] = "/app/filemanager/dir.png";
	
	char galleryBgImg[100] = "/app/gallery/gallerybg.png";
	char gameImg[100] = "/app/game/gamebg.png";
	char quickSettingsBgImg[100] = "/home/menu/quickSettings.png";
	char backiconImg[100] = "/home/icons/backicon.png";
	char homeiconImg[100] = "/home/icons/homeicon.png";
	char multiconImg[100] = "/home/icons/multicon.png";
	char backicon2Img[100] = "/home/icons/backicon2.png";
	char homeicon2Img[100] = "/home/icons/homeicon2.png";
	char multicon2Img[100] = "/home/icons/multicon2.png";
	
	char cursorImg[100] = "/cursor/cursor.png";
	char keyBoardBgImg[100] = "/settings/keyboard.png";
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, aboutBgImg); 
	strcpy(aboutBgPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, developerBgImg); 
	strcpy(developerBgPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, performanceBgImg); 
	strcpy(performanceBgPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, performance2BgImg); 
	strcpy(performance2BgPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, securityBgImg); 
	strcpy(securityBgPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, updatesBgImg); 
	strcpy(updatesBgPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, wifiBgImg); 
	strcpy(wifiBgPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, offSwitchImg); 
	strcpy(offSwitchPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, onSwitchImg); 
	strcpy(onSwitchPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, settingsBgImg); 
	strcpy(settingsBgPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, displayBgImg); 
	strcpy(displayBgPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, apolloBgImg); 
	strcpy(apolloBgPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, nowplayingBgImg); 
	strcpy(nowplayingBgPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, fmBgImg); 
	strcpy(fmBgPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, diriconImg); 
	strcpy(diriconPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, galleryBgImg); 
	strcpy(galleryBgPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, gameImg); 
	strcpy(gameBgPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, quickSettingsBgImg); 
	strcpy(quickSettingsBgPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, backiconImg); 
	strcpy(backiconPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, homeiconImg); 
	strcpy(homeiconPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, multiconImg); 
	strcpy(multiconPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, backicon2Img); 
	strcpy(backicon2Path, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, homeicon2Img); 
	strcpy(homeicon2Path, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, multicon2Img); 
	strcpy(multicon2Path, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, cursorImg); 
	strcpy(cursorPath, tempData);
	
	strcpy(tempData, themeDirPath);
	strcat(tempData, keyBoardBgImg); 
	strcpy(keyBoardBgPath, tempData);
}

void themesReload()
{
	oslDeleteImage(quickSettings);
	oslDeleteImage(backicon);
	oslDeleteImage(homeicon);
	oslDeleteImage(multicon);
	oslDeleteImage(backicon2);
	oslDeleteImage(homeicon2);
	oslDeleteImage(multicon2);
	oslDeleteImage(displaybg);
	oslDeleteImage(cursor);

	displaybg = oslLoadImageFilePNG(displayBgPath, OSL_IN_RAM, OSL_PF_8888);
	quickSettings = oslLoadImageFile(quickSettingsBgPath, OSL_IN_VRAM, OSL_PF_8888);
	backicon = oslLoadImageFilePNG(backiconPath, OSL_IN_RAM, OSL_PF_8888);
	homeicon = oslLoadImageFilePNG(homeiconPath, OSL_IN_RAM, OSL_PF_8888);
	multicon = oslLoadImageFilePNG(multiconPath, OSL_IN_RAM, OSL_PF_8888);
	backicon2 = oslLoadImageFilePNG(backicon2Path, OSL_IN_RAM, OSL_PF_8888);
	homeicon2 = oslLoadImageFilePNG(homeicon2Path, OSL_IN_RAM, OSL_PF_8888);
	multicon2 = oslLoadImageFilePNG(multicon2Path, OSL_IN_RAM, OSL_PF_8888);
	cursor = oslLoadImageFilePNG(cursorPath, OSL_IN_RAM, OSL_PF_8888);
}

void iconPackLoad()
{
	strcpy(appDirPath, setFileDefaultsChar("system/settings/iconpack.bin", "system/icons/Default", appDirPath));
	
	char allappsImg[50] = "/allapps/ic_allapps.png";
	char allapps_pressedImg[50] = "/allapps/ic_allapps_pressed.png";
	char apolloImg[50] = "/music/ic_launcher_apollo.png";
	char browserImg[50] = "/browser/ic_launcher_browser.png";
	char calcImg[50] = "/calculator/ic_launcher_calculator.png";
	char clockImg[50] = "/clock/ic_launcher_clock.png";
	char fmImg[50] = "/filemanager/ic_launcher_filemanager.png";
	char galleryImg[50] = "/gallery/ic_launcher_gallery.png";
	char gameImg[50] = "/game/ic_launcher_game.png";
	char messagesImg[50] = "/messenger/ic_launcher_messenger.png";
	char settingsImg[50] = "/settings/ic_launcher_settings.png";
	char umdImg[50] = "/umd/ic_launcher_umd.png";
	
	strcpy(tempData, appDirPath);
	strcat(tempData, allappsImg); 
	strcpy(allappsPath, tempData);
	
	strcpy(tempData, appDirPath);
	strcat(tempData, allapps_pressedImg); 
	strcpy(allapps_pressedPath, tempData);
	
	strcpy(tempData, appDirPath);
	strcat(tempData, apolloImg); 
	strcpy(apolloPath, tempData);
	
	strcpy(tempData, appDirPath);
	strcat(tempData, browserImg); 
	strcpy(browserPath, tempData);
	
	strcpy(tempData, appDirPath);
	strcat(tempData, calcImg); 
	strcpy(calcPath, tempData);
	
	strcpy(tempData, appDirPath);
	strcat(tempData, clockImg); 
	strcpy(clockPath, tempData);
	
	strcpy(tempData, appDirPath);
	strcat(tempData, fmImg); 
	strcpy(fmPath, tempData);
	
	strcpy(tempData, appDirPath);
	strcat(tempData, galleryImg); 
	strcpy(galleryPath, tempData);
	
	strcpy(tempData, appDirPath);
	strcat(tempData, gameImg); 
	strcpy(gamePath, tempData);
	
	strcpy(tempData, appDirPath);
	strcat(tempData, messagesImg); 
	strcpy(messagesPath, tempData);
	
	strcpy(tempData, appDirPath);
	strcat(tempData, settingsImg); 
	strcpy(settingsPath, tempData);
	
	strcpy(tempData, appDirPath);
	strcat(tempData, umdImg); 
	strcpy(umdPath, tempData);
}

void iconPackReload()
{
	oslDeleteImage(ic_launcher_apollo);
	oslDeleteImage(ic_launcher_browser);
	oslDeleteImage(ic_launcher_calculator);
	oslDeleteImage(ic_launcher_filemanager);
	oslDeleteImage(ic_launcher_gallery);
	oslDeleteImage(ic_launcher_game);
	oslDeleteImage(ic_launcher_messenger);
	oslDeleteImage(ic_launcher_settings);
	oslDeleteImage(ic_launcher_umd);

	ic_launcher_apollo = oslLoadImageFilePNG(apolloPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_browser = oslLoadImageFile(browserPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_calculator = oslLoadImageFilePNG(calcPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_filemanager = oslLoadImageFilePNG(fmPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_gallery = oslLoadImageFilePNG(galleryPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_game = oslLoadImageFilePNG(gamePath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_messenger = oslLoadImageFilePNG(messagesPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_settings = oslLoadImageFilePNG(settingsPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_umd = oslLoadImageFilePNG(umdPath, OSL_IN_RAM, OSL_PF_8888);
}

void settingsControls(int n) //Controls
{
	oslReadKeys();	

	if (pad.Buttons != oldpad.Buttons) 
	{
		if (osl_keys->pressed.down)
		{
			settingsDown();
			timer = 0;
		}
		else if (osl_keys->pressed.up)
		{
			settingsUp();
			timer = 0;
		}
		
		if (osl_keys->pressed.right)
		{
			settingsDownx5();
			timer = 0;
		}
		else if (osl_keys->pressed.left)
		{
			settingsUpx5();	
			timer = 0;
		}
		
		if (osl_keys->pressed.triangle) 
		{
			curScroll = 1;
			current = 1;
		}
		
		if (osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
		}
	}
	
	volumeController();
	
	char * ext = strrchr(folderIcons[current].filePath, '.'); 
	
	if (((ext) != NULL) && (n == 0) && ((strcmp(ext ,".pgf") == 0) || (strcmp(ext ,".PGF") == 0)))
	{
		if ((osl_keys->pressed.cross) && (strcmp(folderIcons[current].filePath, "doesn't matter") != 0))
		{
			changeFont();
		}
	}
	else if (((ext) != NULL) && (n == 1) && ((strcmp(ext ,".png") == 0) || (strcmp(ext ,".jpg") == 0) || (strcmp(ext ,".jpeg") == 0) || (strcmp(ext ,".gif") == 0) || (strcmp(ext ,".PNG") == 0) || (strcmp(ext ,".JPG") == 0) || (strcmp(ext ,".JPEG") == 0) || (strcmp(ext ,".GIF") == 0)))
	{
		if (osl_keys->pressed.cross)
		{
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);	
			showImage(folderIcons[current].filePath, 1);
		}
	}
	
	else if (n == 2)
	{
		if ((osl_keys->pressed.cross) && (strcmp(folderIcons[current].filePath, "doesn't matter") != 0))
		{
			strcpy(appDirPath, folderIcons[current].filePath);
			FILE * iconPackTxt = fopen("system/settings/iconpack.bin", "w");
			fprintf(iconPackTxt,"%s", appDirPath);
			fclose(iconPackTxt);
			iconPackLoad();
			iconPackReload();
		}
	}
	
	else if (n == 3)
	{
		if ((osl_keys->pressed.cross) && (strcmp(folderIcons[current].filePath, "doesn't matter") != 0))
		{
			strcpy(themeDirPath, folderIcons[current].filePath);
			FILE * themeTxt = fopen("system/settings/theme.bin", "w");
			if (strcmp(themeDirPath, "system/themes/Default") == 0)
			{
				fprintf(themeTxt,"system");
				fclose(themeTxt);
				themesLoad();
				themesReload();
			}
			else
			{
				fprintf(themeTxt,"%s", themeDirPath);
				fclose(themeTxt);
				themesLoad();
				themesReload();
			}
		}
	}
	
	else if (n == 4)
	{
		if ((osl_keys->pressed.cross) && (strcmp(folderIcons[current].filePath, "doesn't matter") != 0))
		{
			changeLanguage();
			language = setFileDefaultsInt("system/settings/language.bin", 0, language);
		}
	}
	
	if (osl_keys->pressed.circle && n == 4)
	{	
		oslDeleteImage(displaybg);
		oslDeleteImage(highlight);
		displayMenu();
	}
	
	if (osl_keys->pressed.circle)
	{	
		oslDeleteImage(displaybg);
		oslDeleteImage(highlight);
		displayThemes();
	}
	
	if (osl_keys->pressed.square)
	{
		powermenu();
	}
		
	if (osl_keys->pressed.L)
	{
		oslPlaySound(Lock, 1);  
		lockscreen();
    }
	
	captureScreenshot();
	
	timer++;
	if ((timer > 30) && (pad.Buttons & PSP_CTRL_UP))
	{
		dirDown();
		timer = 25;
	} 
	else if ((timer > 30) && (pad.Buttons & PSP_CTRL_DOWN))
	{
		dirDown();
		timer = 25;
	}

	if (current < 1) 
		current = 1;
	if (current > MAX_FILES) 
		current = MAX_FILES;
}

char * settingsBrowse(const char * path, int n) // n is used here to search for fonts or wallpaper
{
	folderScan(path);
	dirVars();
	
	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));	
		oldpad = pad;
		sceCtrlReadBufferPositive(&pad, 1);
		settingsDisplay();
		
		if (n == 0)
			settingsControls(0); //0 is to used for selecting a font
		else if (n == 1)
			settingsControls(1); // 1 is used for selecting a wallpaper
		else if (n == 2)
			settingsControls(2); // 2 is to used for selecting a folder for iconpacks
		else if (n == 3)
			settingsControls(3); // 3 is to used for selecting a folder for themes
		else if (n == 4)
			settingsControls(4); // 4 is to used for selecting a language
			
		if (strlen(returnMe) > 4) 
			break;
			
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();
	}
	return returnMe;
}

void displaySubThemes(char * browseDirectory, int n) // n is used here to search for fonts or wallpaper
{	
	displaybg = oslLoadImageFilePNG(displayBgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);

	if (!displaybg)
		debugDisplay();
		
	if (n == 0)	
		browseDirectory = settingsBrowse("system/fonts", 0); //For fonts
	else if (n == 1)
		browseDirectory = settingsBrowse("system/framework/framework-res/res", 1); //For wallpapers
	else if (n == 2)
		browseDirectory = settingsBrowse("system/icons", 2); //For icon packs
	else if (n == 3)
		browseDirectory = settingsBrowse("system/themes", 3); //For themes
	else if (n == 4)
		browseDirectory = settingsBrowse("system/settings/language", 4); //For language
	
	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		centerText(480/2, 272/2, browseDirectory, 50);
		
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();	
	}
}

void displayTime()
{	
	FILE * timeSetTxt;
	
	displaybg = oslLoadImageFilePNG(displayBgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG(offSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG(onSwitchPath, OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);

	if (!displaybg)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);

		oslDrawImageXY(displaybg, 0, 0);
		
		oslDrawStringf(20,86, "%s", lang_settingsTime[language][0]);
		
		if (hrTime == 0)
			oslDrawImageXY(offswitch,350,80);
				
		else if (hrTime == 1)
			oslDrawImageXY(onswitch,350,80);
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,86, "%s", lang_settingsTime[language][0]);
			if (hrTime == 0)
			{
				oslDrawImageXY(offswitch,350,80);
				
				if (osl_keys->pressed.cross)
				{
					oslPlaySound(KeypressStandard, 1);
					hrTime = 1;
					timeSetTxt = fopen("system/app/clock/timeSet.bin", "w");
					fprintf(timeSetTxt, "%d", hrTime);
					fclose(timeSetTxt);
				}
			}
			else if (hrTime == 1)
			{
				oslDrawImageXY(onswitch,350,80);
			
				if (osl_keys->pressed.cross)
				{
					oslPlaySound(KeypressStandard, 1);
					hrTime = 0;
					timeSetTxt = fopen("system/app/clock/timeSet.bin", "w");
					fprintf(timeSetTxt, "%d", hrTime);
					fclose(timeSetTxt);
				}
			}
		}

		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			displayMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			displayMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void displayMiscellaneous()
{	
	FILE * widgetActivation;
	FILE * eDesktopActivation;
	FILE * bootAnimActivation;
	FILE * gBootActivation;
	
	bootAnimActivation = fopen("system/boot/bootAnimActivator.bin", "r");
	fscanf(bootAnimActivation,"%d",&bootAnimActivator);
	fclose(bootAnimActivation);

	displaybg = oslLoadImageFilePNG(displayBgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG(offSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG(onSwitchPath, OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);

	if (!displaybg)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);

		oslDrawImageXY(displaybg, 0, 0);
		
		oslDrawStringf(20,86, "%s", lang_settingsMiscellaneous[language][0]);
		oslDrawStringf(20,140, "%s", lang_settingsMiscellaneous[language][1]);
		oslDrawStringf(20,195, "%s", lang_settingsMiscellaneous[language][2]);
		oslDrawStringf(20,245, "%s", lang_settingsMiscellaneous[language][3]);
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,86, "%s", lang_settingsMiscellaneous[language][0]);
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,140, "%s", lang_settingsMiscellaneous[language][1]);
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawStringf(20,195, "%s", lang_settingsMiscellaneous[language][2]);
		}
		
		else if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 216 && cursor->y <= 272)
		{
			oslDrawImageXY(highlight, 0, 228);
			oslDrawStringf(20,245, "%s", lang_settingsMiscellaneous[language][3]);
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);

		if (widgetActivator == 0)
		{
			oslDrawImageXY(offswitch,350,80);
			
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				widgetActivation = fopen("system/widget/widgetActivator.bin", "w");
				widgetActivator = 1;
				fprintf(widgetActivation, "1");
				fclose(widgetActivation);
			}
		}
		
		else if (widgetActivator == 1)
		{
			oslDrawImageXY(onswitch,350,80);
		
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				widgetActivation = fopen("system/widget/widgetActivator.bin", "w");
				widgetActivator = 0;
				fprintf(widgetActivation, "0");
				fclose(widgetActivation);
			}
		}
		
		if (eDesktopActivator == 0)
		{
			oslDrawImageXY(offswitch,350,133);
			
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				eDesktopActivation = fopen("system/home/eDesktopActivator.bin", "w");
				eDesktopActivator = 1;
				fprintf(eDesktopActivation, "1");
				fclose(eDesktopActivation);
			}
		}
		
		else if (eDesktopActivator == 1)
		{
			oslDrawImageXY(onswitch,350,133);
		
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 156 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				eDesktopActivation = fopen("system/home/eDesktopActivator.bin", "w");
				eDesktopActivator = 0;
				fprintf(eDesktopActivation, "0");
				fclose(eDesktopActivation);
			}
		}
		
		if (bootAnimActivator == 0)
		{
			oslDrawImageXY(offswitch,350,186);
			
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				bootAnimActivation = fopen("system/boot/bootAnimActivator.bin", "w");
				bootAnimActivator = 1;
				fprintf(bootAnimActivation, "1");
				fclose(bootAnimActivation);
			}
		}
		
		else if (bootAnimActivator == 1)
		{
			oslDrawImageXY(onswitch,350,186);
		
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 157 && cursor->y <= 215 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				bootAnimActivation = fopen("system/boot/bootAnimActivator.bin", "w");
				bootAnimActivator = 0;
				fprintf(bootAnimActivation, "0");
				fclose(bootAnimActivation);
			}
		}
		
		if (gBootActivator == 0)
		{
			oslDrawImageXY(offswitch,350,236);
			
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 216 && cursor->y <= 272 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				gBootActivation = fopen("system/app/game/boot/gBootActivator.bin", "w");
				gBootActivator = 1;
				fprintf(gBootActivation, "1");
				fclose(gBootActivation);
			}
		}
		
		else if (gBootActivator == 1)
		{
			oslDrawImageXY(onswitch,350,236);
		
			if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 216 && cursor->y <= 272 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				gBootActivation = fopen("system/app/game/boot/gBootActivator.bin", "w");
				gBootActivator = 0;
				fprintf(gBootActivation, "0");
				fclose(gBootActivation);
			}
		}
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			displayMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			displayMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void securityMenu()
{	
	FILE * password;
	FILE * pin;

	securitybg = oslLoadImageFilePNG(securityBgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG(offSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG(onSwitchPath, OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	if (!securitybg || !highlight)
		debugDisplay();
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslDrawImageXY(securitybg, 0, 0);
		
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);
		
		oslDrawStringf(20,83, "%s", lang_settingsSecuirty[language][0]); 
		oslDrawStringf(20,144, "%s", lang_settingsSecuirty[language][1]); 
		
		if (cursor->x  >= 0 && cursor->x  <= 444 && cursor->y >= 61 && cursor->y <= 118)
		{	
			oslDrawImageXY(highlight, 0, 62);
			oslDrawStringf(20,83, "%s", lang_settingsSecuirty[language][0]); 
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);
				sceIoRemove("system/settings/password.bin");
				sceIoRemove("system/settings/pin.bin");
				openOSK("Enter Password", "", 20, -1);
				password = fopen("system/settings/password.bin", "w");
				fprintf(password, "%s", tempMessage);
				fclose(password);
			}
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 119 && cursor->y <= 178)
		{	
			oslDrawImageXY(highlight, 0, 120);
			oslDrawStringf(20,144, "%s", lang_settingsSecuirty[language][1]); 
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);
				sceIoRemove("system/settings/password.bin");
				sceIoRemove("system/settings/pin.bin");
				openOSK("Enter Pin", "", 5, -1);
				pin = fopen("system/settings/pin.bin", "w");
				fprintf(pin, "%s", tempPin);
				fclose(pin);
			}
		}

		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(securitybg);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			settingsMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(securitybg);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(securitybg);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void wifiMenu()
{	
	wifibg = oslLoadImageFilePNG(wifiBgPath, OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG(offSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG(onSwitchPath, OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);

	if (!wifibg)
		debugDisplay();
		
	int enabled = 1;
    int selectedConfig = 0;
	int wifi_y = 95;
	static char buffer[100] = "";
	
	//Get connections list:
    struct oslNetConfig configs[OSL_MAX_NET_CONFIGS];
    int numconfigs = oslGetNetConfigs(configs);
	if (!numconfigs)
	{
        sprintf(Settings_message, "%s", lang_settingsWifi[language][0]);
        enabled = 0;
    }
	
	oslNetInit(); 
	
	if (!oslIsWlanPowerOn())
        sprintf(Settings_message, "%s", lang_settingsWifi[language][1]);

	while (!osl_quit)
	{			
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(wifibg, 0, 0);
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);

		if (enabled)
		{
			sprintf(buffer, "%s", configs[selectedConfig].name);
    		oslDrawStringf(10, wifi_y+28, configs[selectedConfig].name);
			oslDrawStringf(10, 206, "%s", lang_settingsWifi[language][2]);
        }
		
		oslDrawStringf(10, 220, Settings_message);
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		switchStatus(3);
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		
		if (osl_keys->released.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			connectToAP(selectedConfig + 1);
        }
		else if (osl_keys->released.up)
		{
			if (++selectedConfig >= numconfigs)
				selectedConfig = numconfigs - 1;
        } 
		else if (osl_keys->released.down)
		{
			 if (--selectedConfig < 0)
				selectedConfig = 0;
        }
		
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{	
			oslDeleteImage(wifibg);	
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			settingsMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(wifibg);	
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(wifibg);	
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
	oslNetTerm();
}

void DumpBootBin(void)
{
	int i;
	int fd;

	oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
	i = sceUmdCheckMedium();
	if(i == 0)
	{
		oslDrawStringf(4,5, "%s", lang_settingsDump[language][5]);
		i = sceUmdWaitDriveStat(PSP_UMD_PRESENT);
	}

	i = sceUmdActivate(1, "disc0:");
	oslDrawStringf(4,5, "%s", lang_settingsDump[language][6]);
	oslSyncFrame();
	sceKernelDelayThread(2*1000000);

	i = sceUmdWaitDriveStat(PSP_UMD_READY);

	/* Open the UMD_DATA.BIN */
	fd = sceIoOpen("disc0:/UMD_DATA.BIN", PSP_O_RDONLY, 0777);
	if(fd >= 0)
	{
		char game_id[11];
		char path[256];

		sceIoRead(fd, game_id, 10);
		sceIoClose(fd);
		game_id[10] = 0;
		build_path(path, "ms0:/", game_id, 0);
		sceIoMkdir(path, 0777);

		oslDrawStringf(4,15,"%s %s\n", lang_settingsDump[language][7], game_id);
		write_file("disc0:/PSP_GAME/SYSDIR", path, "BOOT.BIN");
		oslSyncFrame();
		sceKernelDelayThread(3*1000000);
	}
}

/* Dump a filing system */
void dump_filesystem(const char *root, const char *write_loc)
{
	int dfd;
	char next_root[256];
	char next_write[256];

	sceIoMkdir(write_loc, 0777);

	dfd = sceIoDopen(root);
	if(dfd > 0)
	{
		SceIoDirent dir;

		while(sceIoDread(dfd, &dir) > 0)
		{
			if(dir.d_stat.st_attr & FIO_SO_IFDIR)
			{
				if(dir.d_name[0] != '.')
				{
					build_path(next_write, write_loc, dir.d_name, 0);
					build_path(next_root, root, dir.d_name, 1);
					dump_filesystem(next_root, next_write);
				}
			}
			else
			{
				write_file(root, write_loc, dir.d_name);
			}
		}
		sceIoDclose(dfd);
	}
}

/* Dump memory */
void dump_memory(void)
{
	int fd;

	oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
	oslDrawStringf(4,5,"%s 0x8400000\n", lang_settingsDump[language][8]);
	fd = sceIoOpen("ms0:/MEMORY.BIN", PSP_O_CREAT | PSP_O_TRUNC | PSP_O_WRONLY, 0777);
	if(fd >= 0)
	{
		sceIoWrite(fd, (void *) 0x8400000, 28*1024*1024);
		sceIoClose(fd);
	}
	oslSyncFrame();
	sceKernelDelayThread(3*1000000);
}

void dumpMenu()
{
	developerbg = oslLoadImageFilePNG(developerBgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG(offSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG(onSwitchPath, OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	if (!developerbg || !highlight)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(developerbg, 0, 0);
		switchStatus(2);
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);

		oslDrawStringf(20,123, "%s", lang_settingsDump[language][0]);
		oslDrawStringf(20,181, "%s", lang_settingsDump[language][1]);
		oslDrawStringf(20,236, "%s", lang_settingsDump[language][2]);
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 99 && cursor->y <= 155)
		{
			oslDrawImageXY(highlight, 0, 101);
			oslDrawStringf(20,123, "%s", lang_settingsDump[language][0]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				dump_filesystem("flash0:/", "ms0:/flash0");
			}
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 156 && cursor->y <= 211)
		{
			oslDrawImageXY(highlight, 0, 158);
			oslDrawStringf(20,181, "%s", lang_settingsDump[language][1]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				dump_filesystem("flash1:/", "ms0:/flash1");
			}
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 212 && cursor->y <= 268)
		{
			oslDrawImageXY(highlight, 0, 216);
			oslDrawStringf(20,236, "%s", lang_settingsDump[language][2]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(developerbg);
				oslDeleteImage(highlight);
				oslDeleteImage(offswitch);	
				oslDeleteImage(onswitch);
				dumpMenuMore();
			}
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			developerMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			developerMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}	
}

void dumpMenuMore()
{
	developerbg = oslLoadImageFilePNG(developerBgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG(offSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG(onSwitchPath, OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	if (!developerbg || !highlight)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(developerbg, 0, 0);
		switchStatus(2);
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);

		oslDrawStringf(20,123, "%s", lang_settingsDump[language][3]);
		oslDrawStringf(20,181, "%s", lang_settingsDump[language][4]);
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 99 && cursor->y <= 155)
		{
			oslDrawImageXY(highlight, 0, 101);
			oslDrawStringf(20,123, "%s", lang_settingsDump[language][3]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				DumpBootBin();
			}
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 156 && cursor->y <= 211)
		{
			oslDrawImageXY(highlight, 0, 158);
			oslDrawStringf(20,181, "%s", lang_settingsDump[language][4]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				dump_memory();
			}
		}

		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			dumpMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			dumpMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}	
}

SceUID modules[3];

void developerMenu()
{	
	developerbg = oslLoadImageFilePNG(developerBgPath, OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG(offSwitchPath, OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG(onSwitchPath, OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	if (!developerbg || !highlight)
		debugDisplay();
		
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(developerbg, 0, 0);
		
		switchStatus(2);
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);
		
		oslDrawStringf(10,107, "%s", lang_settingsDeveloperOptions[language][0]);
		oslDrawStringf(10,121, "%s", lang_settingsDeveloperOptions[language][1]);
		oslDrawStringf(10,135, "%s", lang_settingsDeveloperOptions[language][2]);
		oslDrawStringf(10,168, "%s", lang_settingsDeveloperOptions[language][3]);
		oslDrawStringf(10,182, "%s", lang_settingsDeveloperOptions[language][2]);
		oslDrawStringf(10,236, "%s", lang_settingsDeveloperOptions[language][4]);
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 99 && cursor->y <= 154)
		{
			oslDrawImageXY(highlight, 0, 100);
			oslDrawStringf(10,107, "%s", lang_settingsDeveloperOptions[language][0]);
			oslDrawStringf(10,121, "%s", lang_settingsDeveloperOptions[language][1]);
			oslDrawStringf(10,135, "%s", lang_settingsDeveloperOptions[language][2]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				RJL = 1;
				modules[0] = loadStartModule("modules/RemoteJoyLite.prx");
			}
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 155 && cursor->y <= 214)
		{
			oslDrawImageXY(highlight, 0, 157);
			oslDrawStringf(10,168, "%s", lang_settingsDeveloperOptions[language][3]);
			oslDrawStringf(10,182, "%s", lang_settingsDeveloperOptions[language][2]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				PSPDebug = 1;
				modules[1] = loadStartModule("modules/psplink.prx");
			}
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 215 && cursor->y <= 272)
		{
			oslDrawImageXY(highlight, 0, 216);
			oslDrawStringf(10,236, "%s", lang_settingsDeveloperOptions[language][4]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(developerbg);
				oslDeleteImage(highlight);
				oslDeleteImage(offswitch);	
				oslDeleteImage(onswitch);
				dumpMenu();
			}
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();

		if(RJL == 1 && osl_keys->pressed.triangle)
		{
			stopUnloadModule(modules[0]);
		}

		if(PSPDebug == 1  && osl_keys->pressed.triangle)
		{	
			stopUnloadModule(modules[1]);
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}	
}

void settingsHighlight()
{		
	if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 98 && cursor->y <= 154)
	{
		oslDrawImageXY(wifi, 1, 100);
		oslDrawStringf(50,120,"%s", lang_settingsMain[language][0]);
	}
		
	if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 98 && cursor->y <= 154)
	{
		oslDrawImageXY(security, 222, 101);
		oslDrawStringf(280,120,"%s", lang_settingsMain[language][1]);
	}
	
	if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 155 && cursor->y <= 210)
	{
		oslDrawImageXY(themes, 0, 156);
		oslDrawStringf(50,177,"%s", lang_settingsMain[language][2]);
	}

	if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 155 && cursor->y <= 210)
	{
		oslDrawImageXY(performance, 225, 155);
		oslDrawStringf(280,177,"%s", lang_settingsMain[language][3]);
	}
		
	if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 211 && cursor->y <= 267)
	{
		oslDrawImageXY(developeroptions, 0, 211);
		oslDrawStringf(50,234,"%s", lang_settingsMain[language][4]);
	}

	if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 211 && cursor->y <= 267)
	{
		oslDrawImageXY(about, 226, 211);
		oslDrawStringf(280,234,"%s", lang_settingsMain[language][5]);
	}
}

void settingsDeleteResources()
{
	oslDeleteImage(settingsbg);
	oslDeleteImage(about);
	oslDeleteImage(themes);
	oslDeleteImage(developeroptions);
	oslDeleteImage(wifi);
	oslDeleteImage(performance);
	oslDeleteImage(security);
}

void settingsMenu()
{
	struct apps activateApp;

	activateApp.settings = 1;
	
	themesLoad();
	
	settingsbg = oslLoadImageFilePNG(settingsBgPath, OSL_IN_RAM, OSL_PF_8888);
	about = oslLoadImageFilePNG("system/settings/about.png", OSL_IN_RAM, OSL_PF_8888);
	themes = oslLoadImageFilePNG("system/settings/themes.png", OSL_IN_RAM, OSL_PF_8888);
	developeroptions = oslLoadImageFilePNG("system/settings/developeroptions.png", OSL_IN_RAM, OSL_PF_8888);
	wifi = oslLoadImageFilePNG("system/settings/wifi.png", OSL_IN_RAM, OSL_PF_8888);
	security = oslLoadImageFilePNG("system/settings/security.png", OSL_IN_RAM, OSL_PF_8888);
	performance = oslLoadImageFilePNG("system/settings/performance.png", OSL_IN_RAM, OSL_PF_8888);
	
	oslSetFont(Roboto);

	if (!settingsbg || !about || !developeroptions || !wifi || !themes || !performance || !security)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(settingsbg, 0, 0);
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);

		oslDrawStringf(50,120,"%s", lang_settingsMain[language][0]);
		oslDrawStringf(280,120,"%s", lang_settingsMain[language][1]);
		oslDrawStringf(50,177,"%s", lang_settingsMain[language][2]);
		oslDrawStringf(280,177,"%s", lang_settingsMain[language][3]);
		oslDrawStringf(50,234,"%s", lang_settingsMain[language][4]);
		oslDrawStringf(280,234,"%s", lang_settingsMain[language][5]);
		
		settingsHighlight();
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{	
			settingsDeleteResources();
			appdrawer();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
	
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsDeleteResources();
			appdrawer();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsDeleteResources();
			home();
		}
		
		if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 98 && cursor->y <= 154 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsDeleteResources();
			wifiMenu();
		}
		
		if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 98 && cursor->y <= 154 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsDeleteResources();
			securityMenu();
		}
		
		if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 155 && cursor->y <= 210 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsDeleteResources();
			displayMenu();
		}

		if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 155 && cursor->y <= 210 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsDeleteResources();
			performanceMenu();
		}
		
		if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 211 && cursor->y <= 267 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsDeleteResources();
			developerMenu();
		}

		if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 211 && cursor->y <= 267 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			settingsDeleteResources();
			aboutMenu();
		}

		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}