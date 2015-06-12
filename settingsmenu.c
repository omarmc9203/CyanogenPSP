#include "settingsmenu.h"
#include "language.h"
#include "include/pgeZip.h"
#include "include/ram.h"
#include "include/utils.h"
#include "include/pspusbdevice.h"
#include "include/common.h"
#include "home.h"
#include "appdrawer.h"
#include "fm.h"
#include "clock.h"
#include "game.h"
#include "gallery.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "recoverymenu.h"
#include "screenshot.h"

char name;
int setclock;
char cyanogenpspversion[5] = "5.2";
char lang[12] = "Uk English";
static char Settings_message[100] = "";

char buffer[100] = "";

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
	
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!recoverybg)
		debugDisplay();
	
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
					oslDrawImageXY(recoverybg, 0, 0);
					oslDrawStringf(10,10,"Flashing zip...");
					if (fileExists("ms0:/PSP/GAME/CyanogenPSP/Boot.zip"))
					{
					pgeZip* zipFiles = pgeZipOpen("../Boot.zip");
					chdir("..");
					pgeZipExtract(zipFiles, NULL);
					pgeZipClose(zipFiles);
					oslDrawStringf(10,30,"Installed Successfully.");
					oslDrawStringf(10,50,"Exiting..");
					oslSyncFrame();
					sceKernelDelayThread(2*1000000);
					oslSyncFrame();
					oslDrawStringf(10,50,"Enjoy :)");
					sceKernelDelayThread(3*1000000);
					oslDeleteImage(recoverybg);
					sceIoRemove("ms0:/PSP/GAME/CyanogenPSP/Boot/system/build.prop");
					sceKernelExitGame();
					}
					oslDeleteImage(recoverybg);
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
            oslBrowserInit("http://raw.githubusercontent.com/joel16/CyanogenPSP-Bin/master/Boot.zip", "/PSP/GAME/CyanogenPSP", 5*1024*1024,
                                         PSP_UTILITY_HTMLVIEWER_DISPLAYMODE_SMART_FIT,
                                         PSP_UTILITY_HTMLVIEWER_DISABLE_STARTUP_LIMITS,
                                         PSP_UTILITY_HTMLVIEWER_INTERFACEMODE_FULL,
                                         PSP_UTILITY_HTMLVIEWER_CONNECTMODE_MANUAL_ALL);
			memset(message, 0, sizeof(message));

        }
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
    }
	oslNetTerm();
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

void loadConfig()
{	
	//load/open file
	FILE * configtxt = fopen(configFile, "rb");

	if(!configtxt) 
		return saveConfig();
	
	//close file
	fclose(configtxt);
}

void saveConfig()
{
	FILE * configtxt = fopen(configFile, "wb"); //create config file if it doesn't exist.
	fprintf(configtxt,
"ro.build.version.release = %s\r\n\
ro.product.model = %d\r\n\
ro.product.locale.language = %s\r\n\
ro.build.user = Joel16\r\n\
ro.product.cpu.frequency =  %d\r\n\
ro.product.bus.frequency =  %d\r\n\
ro.build.date = Sat Mar 08 9:31 AM EST 2015",
	cyanogenpspversion, kuKernelGetModel(),lang,getCpuClock(),getBusClock());
	fclose(configtxt);	
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
	char pspmodel = kuKernelGetModel();
	
	if(pspmodel == 0)
	{
		oslDrawStringf(x,y,"Model: PSP 1000");
	}
   
	else if(pspmodel == 1)
	{
		oslDrawStringf(x,y,"Model: PSP 2000");
	}
   
	else if (pspmodel == 2)
	{
		oslDrawStringf(x,y,"Model: PSP 3000");
	}
   
	else if(pspmodel == 3)
	{
		oslDrawStringf(x,y,"Model: PSP 3000");
	}
		
	else if (pspmodel == 4)
	{
		oslDrawStringf(x,y,"Model: PSP Go N1000");
	}
   
	else
	{
		oslDrawStringf(x,y,"Model: PS Vita");
	}
}

fw_version getFwVersion(fw_version *v);

void aboutMenu()
{	
	aboutbg = oslLoadImageFilePNG("system/settings/aboutbg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	if (!aboutbg || !highlight)
		debugDisplay();

	u8 *macAddress = oslAdhocGetMacAddress();
	
	fw_version version;
	getFwVersion(&version);

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
		oslDrawStringf(20,129,"%s %s-20150308-%s", lang_settingsAbout[language][2], cyanogenpspversion, lang_settingsAbout[language][3]);
		oslDrawStringf(20,157,"%s %02X:%02X:%02X:%02X:%02X:%02X", lang_settingsAbout[language][4], macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
		oslDrawStringf(20,185,"%s %d.%d", lang_settingsAbout[language][5], version.major, version.minor);
		oslDrawStringf(20,199,"%s %s", lang_settingsAbout[language][6], OSL_VERSION);
		oslDrawStringf(20,213,"joellovesanna@psp #1");

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
			oslDrawStringf(20,129,"%s %s-20150308-%s", lang_settingsAbout[language][2], cyanogenpspversion, lang_settingsAbout[language][3]);
			oslDrawStringf(20,157,"%s %02X:%02X:%02X:%02X:%02X:%02X", lang_settingsAbout[language][4], macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void updatesMenu()
{		    
	updatesbg = oslLoadImageFilePNG("system/settings/updatesbg.png", OSL_IN_RAM, OSL_PF_8888);
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
		
		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
	oslNetTerm();
}

void performanceMenu()
{	
	performancebg = oslLoadImageFilePNG("system/settings/performancebg.png", OSL_IN_RAM, OSL_PF_8888);
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

		oslDrawStringf(15,103, "%s", lang_settingsPerformance[language][0]);
		oslDrawStringf(15,166, "%s", lang_settingsPerformance[language][1]);
		oslDrawStringf(15,240, "%s", lang_settingsPerformance[language][2]);
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 75 && cursor->y <= 133)
		{
			oslDrawImageXY(highlight, 0, 86);
			oslDrawStringf(15,103, "%s", lang_settingsPerformance[language][0]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(highlight);
				oslDeleteImage(performancebg);
				processorMenu();
			}
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 135 && cursor->y <= 191)
		{
			oslDrawImageXY(highlight, 0, 144);
			oslDrawStringf(15,166, "%s", lang_settingsPerformance[language][1]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(highlight);
				oslDeleteImage(performancebg);
				ramMenu();
			}
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 229 && cursor->y <= 272)
		{
			oslDrawImageXY(highlight, 0, 229);
			oslDrawStringf(15,240, "%s", lang_settingsPerformance[language][2]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(highlight);
				oslDeleteImage(performancebg);
				storageMenu();
			}
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void setCpuBoot()
{
	FILE * processorInfo;
	
	if (!(fileExists("system/settings/processorInfo.bin")))
	{
		processorInfo = fopen("system/settings/processorInfo.bin", "w");
		fprintf(processorInfo, "4");
		fclose(processorInfo);
	}

	processorInfo = fopen("system/settings/processorInfo.bin", "r");
	fscanf(processorInfo,"%d",&processorState);
	fclose(processorInfo);
	
	if (processorState == 0)
		scePowerSetClockFrequency(20, 20, 10);
	else if (processorState == 1)
		scePowerSetClockFrequency(75, 75, 37);
	else if (processorState == 2)
		scePowerSetClockFrequency(100, 100, 50);
	else if (processorState == 3)
		scePowerSetClockFrequency(133, 133, 66);
	else if (processorState == 4)
		scePowerSetClockFrequency(222, 222, 111);
	else if (processorState == 5)
		scePowerSetClockFrequency(266, 266, 133);
	else if (processorState == 6)
		scePowerSetClockFrequency(300, 300, 150);
	else if (processorState == 7)
		scePowerSetClockFrequency(333, 333, 166);
}

void processorMenu()
{	
	FILE * processorInfo;
	
	int currentState = stateOff;
	int cpufreq, cpu, bus, state = 0;
	
	processorbg = oslLoadImageFilePNG("system/settings/processorbg.png", OSL_IN_RAM, OSL_PF_8888);
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
			oslDrawImageXY(highlight, 0, 122);
			oslDrawStringf(20,128, "%s", lang_settingsProcessor[language][1]);
			oslDrawStringf(20,145, "%s", lang_settingsProcessor[language][2]);
		}
		
		if (osl_keys->pressed.L)
		{
			currentState = stateUnderClock;
		}
		else if (osl_keys->pressed.R)
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
		
		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void ramMenu()
{	
	performancebg = oslLoadImageFilePNG("system/settings/performancebg2.png", OSL_IN_RAM, OSL_PF_8888);
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
		
		oslDrawStringf(20,98,"%s %d %s\n", lang_settingsRAM[language][0], oslGetRamStatus().maxAvailable/1000000, lang_settingsRAM[language][1]); 
	
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void storageMenu()
{	
	performancebg = oslLoadImageFilePNG("system/settings/performancebg2.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	unsigned int buf[5]; 
	unsigned int *pbuf = buf; 
	sceIoDevctl("ms0:", 0x02425818, &pbuf, sizeof(pbuf), 0, 0);
	
	double freeSpace = buf[1]*buf[3]*buf[4];
	double totalSpace = buf[0]*buf[3]*buf[4];
	
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

		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void displayMenu()
{	
	displaybg = oslLoadImageFilePNG("system/settings/displaybg.png", OSL_IN_RAM, OSL_PF_8888);
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
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 177)
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
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 178 && cursor->y <= 228)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawStringf(20,195, "%s", lang_settingsDisplay[language][2]);
			if (osl_keys->pressed.cross)
			{	
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displayMiscellaneous();
			}
		}

		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void displayThemes()
{	
	displaybg = oslLoadImageFilePNG("system/settings/displaybg.png", OSL_IN_RAM, OSL_PF_8888);
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
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawStringf(20,86, "%s", lang_settingsThemes[language][0]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displaySubThemes("system/framework/framework-res/res", 1);
			}
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 177)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,140, "%s", lang_settingsThemes[language][1]);
			if (osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(displaybg);
				oslDeleteImage(highlight);			
				displaySubThemes("system/fonts", 0);
			}
		}

		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
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
		FILE * fontPathTXT = fopen("system/settings/fonts.txt", "w");
		fprintf(fontPathTXT,"%s", folderIcons[current].filePath);
		fclose(fontPathTXT);
		Roboto = oslLoadIntraFontFile(folderIcons[current].filePath, INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
		oslSetFont(Roboto);
		return;
	}
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
		
		if (osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			openDir(folderIcons[current].filePath, folderIcons[current].fileType);
		}
	}
	
	char * ext = strrchr(folderIcons[current].filePath, '.'); 
	
	if (((ext) != NULL) && (n == 0) && ((strcmp(ext ,".pgf") == 0) || (strcmp(ext ,".PGF") == 0)))
	{
		if (osl_keys->pressed.cross)
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
	
	if (osl_pad.held.R && osl_keys->pressed.triangle)
	{
		screenshot();
	}
	
	timer++;
	if ((timer > 30) && (pad.Buttons & PSP_CTRL_UP)) {
		dirDown();
		timer = 25;
	} else if ((timer > 30) && (pad.Buttons & PSP_CTRL_DOWN)) {
		dirDown();
		timer = 25;
	}

	if (current < 1) current = 1; // Stop the ">" from moving past the minimum files
	if (current > MAX_FILES) current = MAX_FILES; // Stop the ">" from moving past the max files

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
			settingsControls(0); //0 is to use the controls for selecting a font
		else if (n == 1)
			settingsControls(1); // 1 is to use the controls for selecting a wallpaper
		
		sceDisplayWaitVblankStart();
		
		if (strlen(returnMe) > 4) {
			break;
		}
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();
	}
	return returnMe;
}

void displaySubThemes(char * browseDirectory, int n) // n is used here to search for fonts or wallpaper
{	
	displaybg = oslLoadImageFilePNG("system/settings/displaybg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);

	if (!displaybg)
		debugDisplay();
		
	if (n == 0)	
		browseDirectory = settingsBrowse("system/fonts", 0); //For fonts
	else if (n == 1)
		browseDirectory = settingsBrowse("system/framework/framework-res/res", 1); //For wallpapers
	
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
	
	displaybg = oslLoadImageFilePNG("system/settings/displaybg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG("system/settings/offswitch.png", OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG("system/settings/onswitch.png", OSL_IN_RAM, OSL_PF_8888);

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
					timeSetTxt = fopen("system/app/clock/timeSet.txt", "w");
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
					timeSetTxt = fopen("system/app/clock/timeSet.txt", "w");
					fprintf(timeSetTxt, "%d", hrTime);
					fclose(timeSetTxt);
				}
			}
		}

		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
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
	
	bootAnimActivation = fopen("system/boot/bootAnimActivator.txt", "r");
	fscanf(bootAnimActivation,"%d",&bootAnimActivator);
	fclose(bootAnimActivation);

	displaybg = oslLoadImageFilePNG("system/settings/displaybg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG("system/settings/offswitch.png", OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG("system/settings/onswitch.png", OSL_IN_RAM, OSL_PF_8888);

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
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 177)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawStringf(20,140, "%s", lang_settingsMiscellaneous[language][1]);
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 178 && cursor->y <= 227)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawStringf(20,195, "%s", lang_settingsMiscellaneous[language][2]);
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 228 && cursor->y <= 272)
		{
			oslDrawImageXY(highlight, 0, 228);
			oslDrawStringf(20,245, "%s", lang_settingsMiscellaneous[language][3]);
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);

		if (widgetActivator == 0)
		{
			oslDrawImageXY(offswitch,350,80);
			
			if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				widgetActivation = fopen("system/widget/widgetactivator.txt", "w");
				widgetActivator = 1;
				fprintf(widgetActivation, "1");
				fclose(widgetActivation);
			}
		}
		
		else if (widgetActivator == 1)
		{
			oslDrawImageXY(onswitch,350,80);
		
			if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				widgetActivation = fopen("system/widget/widgetactivator.txt", "w");
				widgetActivator = 0;
				fprintf(widgetActivation, "0");
				fclose(widgetActivation);
			}
		}
		
		if (eDesktopActivator == 0)
		{
			oslDrawImageXY(offswitch,350,133);
			
			if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 177 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				eDesktopActivation = fopen("system/home/eDesktopActivator.txt", "w");
				eDesktopActivator = 1;
				fprintf(eDesktopActivation, "1");
				fclose(eDesktopActivation);
			}
		}
		
		else if (eDesktopActivator == 1)
		{
			oslDrawImageXY(onswitch,350,133);
		
			if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 177 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				eDesktopActivation = fopen("system/home/eDesktopActivator.txt", "w");
				eDesktopActivator = 0;
				fprintf(eDesktopActivation, "0");
				fclose(eDesktopActivation);
			}
		}
		
		if (bootAnimActivator == 0)
		{
			oslDrawImageXY(offswitch,350,186);
			
			if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 178 && cursor->y <= 227 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				bootAnimActivation = fopen("system/boot/bootAnimActivator.txt", "w");
				bootAnimActivator = 1;
				fprintf(bootAnimActivation, "1");
				fclose(bootAnimActivation);
			}
		}
		
		else if (bootAnimActivator == 1)
		{
			oslDrawImageXY(onswitch,350,186);
		
			if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 178 && cursor->y <= 227 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				bootAnimActivation = fopen("system/boot/bootAnimActivator.txt", "w");
				bootAnimActivator = 0;
				fprintf(bootAnimActivation, "0");
				fclose(bootAnimActivation);
			}
		}
		
		if (gBootActivator == 0)
		{
			oslDrawImageXY(offswitch,350,236);
			
			if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 228 && cursor->y <= 272 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				gBootActivation = fopen("system/app/game/boot/gBootActivator.txt", "w");
				gBootActivator = 1;
				fprintf(gBootActivation, "1");
				fclose(gBootActivation);
			}
		}
		
		else if (gBootActivator == 1)
		{
			oslDrawImageXY(onswitch,350,236);
		
			if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 228 && cursor->y <= 272 && osl_keys->pressed.cross)
			{
				oslPlaySound(KeypressStandard, 1);  
				gBootActivation = fopen("system/app/game/boot/gBootActivator.txt", "w");
				gBootActivator = 0;
				fprintf(gBootActivation, "0");
				fclose(gBootActivation);
			}
		}
		
		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void securityMenu()
{	
	FILE * password;
	FILE * pin;

	securitybg = oslLoadImageFilePNG("system/settings/securitybg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG("system/settings/offswitch.png", OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG("system/settings/onswitch.png", OSL_IN_RAM, OSL_PF_8888);

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

		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

void wifiMenu()
{	
	wifibg = oslLoadImageFilePNG("system/settings/wifibg.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG("system/settings/offswitch.png", OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG("system/settings/onswitch.png", OSL_IN_RAM, OSL_PF_8888);

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
		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
		
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}	
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
	developerbg = oslLoadImageFilePNG("system/settings/developerbg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG("system/settings/offswitch.png", OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG("system/settings/onswitch.png", OSL_IN_RAM, OSL_PF_8888);

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
		
		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}	
}

void dumpMenuMore()
{
	developerbg = oslLoadImageFilePNG("system/settings/developerbg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG("system/settings/offswitch.png", OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG("system/settings/onswitch.png", OSL_IN_RAM, OSL_PF_8888);

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
		
		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}	
}

void developerMenu()
{
	developerbg = oslLoadImageFilePNG("system/settings/developerbg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG("system/settings/offswitch.png", OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG("system/settings/onswitch.png", OSL_IN_RAM, OSL_PF_8888);

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
				modules[0] = LoadStartModule("modules/RemoteJoyLite.prx");
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
				modules[1] = LoadStartModule("modules/psplink.prx");
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
		
		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}

		if(RJL == 1 && osl_keys->pressed.triangle)
		{
			StopUnloadModule(modules[0]);
		}

		if(PSPDebug == 1  && osl_keys->pressed.triangle)
		{	
			StopUnloadModule(modules[1]);
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
		oslDrawStringf(280,233,"%s", lang_settingsMain[language][5]);
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
	settingsbg = oslLoadImageFilePNG("system/settings/settingsbg.png", OSL_IN_RAM, OSL_PF_8888);
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
		
		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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

		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}