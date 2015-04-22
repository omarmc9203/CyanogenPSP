#include "settingsmenu.h"
#include "include/pgeZip.h"
#include "include/ram.h"
#include "include/utils.h"
#include "include/pspusbdevice.h"
#include "home.h"
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
            int res = oslBrowserInit("http://raw.githubusercontent.com/joel16/CyanogenPSP-Bin/master/Boot.zip", "/PSP/GAME/CyanogenPSP", 5*1024*1024,
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
			oslDrawString(58,74,"Off");
		}
		
		else
		{
			oslDrawImageXY(onswitch, 392, 68);
			oslDrawString(58,74,"On");
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
		oslDrawString(x,y,"Model: PSP 1000");
	}
   
	else if(pspmodel == 1)
	{
		oslDrawString(x,y,"Model: PSP 2000");
	}
   
	else if (pspmodel == 2)
	{
		oslDrawString(x,y,"Model: PSP 3000");
	}
   
	else if(pspmodel == 3)
	{
		oslDrawString(x,y,"Model: PSP 3000");
	}
		
	else if (pspmodel == 4)
	{
		oslDrawString(x,y,"Model: PSP Go N1000");
	}
   
	else
	{
		oslDrawString(x,y,"Model: PS Vita");
	}
}

typedef struct
{
	unsigned int major;
	unsigned int minor;
} fw_version;

void aboutMenu()
{	
	aboutbg = oslLoadImageFilePNG("system/settings/aboutbg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	if (!aboutbg || !highlight)
		debugDisplay();

	int n = 0;
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

		oslDrawString(20,78,"CyanogenPSP Updates");
		oslDrawString(20,92,"Click for, view or install available updates");
		pspGetModel(20,143);
		oslDrawStringf(20,129,"CyanogenPSP version: %s-20150308-OFFICIAL",cyanogenpspversion);
		oslDrawStringf(20,157,"Mac Address: %02X:%02X:%02X:%02X:%02X:%02X", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
		oslDrawStringf(20,185,"Kernel version: %d.%d", version.major, version.minor);
		oslDrawStringf(20,199,"OSLib version: %s",OSL_VERSION);
		oslDrawString(20,213,"joellovesanna@psp #1");

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 62 && cursor->y <= 119)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawString(20,78,"CyanogenPSP Updates");
			oslDrawString(20,92,"Click for, view or install available updates");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 120 && cursor->y <= 178)
		{
			oslDrawImageXY(highlight, 0, 122);
			pspGetModel(20,143);
			oslDrawStringf(20,129,"CyanogenPSP version: %s-20150308-OFFICIAL",cyanogenpspversion);
			oslDrawStringf(20,157,"Mac Address: %02X:%02X:%02X:%02X:%02X:%02X", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0);
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

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 62 && cursor->y <= 119 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			updatesMenu();
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 121 && cursor->y <= 178 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			if  (osl_keys->pressed.cross)
				n+=1;
			if (n == 4)
			{
				oslDeleteImage(aboutbg);
				oslDeleteImage(highlight);
				easterEgg();
			}
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

void easterEgg()
{
	easterEggImg = oslLoadImageFilePNG("system/settings/easteregg.png", OSL_IN_RAM, OSL_PF_8888);
		
	if (!easterEggImg)
		debugDisplay();
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		oslClearScreen(RGB(0,0,0));
		controls();
		oslDrawImageXY(easterEggImg, 0, 0);
		
		if(osl_keys->pressed.circle)
		{
			oslDeleteImage(easterEggImg);
			aboutMenu();
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
		
		oslDrawString(20,93,"Check for Updates");

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 63 && cursor->y <= 113)
		{
			oslDrawImageXY(highlight, 0, 70);
			oslDrawString(20,93,"Check for Updates");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 63 && cursor->y <= 113 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			onlineUpdater();
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0);
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

		oslDrawString(15,103,"Processor");
		oslDrawString(15,166,"Ram Management");
		oslDrawString(15,240,"Storage Management");
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 75 && cursor->y <= 133)
		{
			oslDrawImageXY(highlight, 0, 86);
			oslDrawString(15,103,"Processor");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 135 && cursor->y <= 191)
		{
			oslDrawImageXY(highlight, 0, 144);
			oslDrawString(15,166,"Ram Management");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 229 && cursor->y <= 272)
		{
			oslDrawImageXY(highlight, 0, 229);
			oslDrawString(15,240,"Storage Management");
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0);
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
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 75 && cursor->y <= 133 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			processorMenu();
		}

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 135 && cursor->y <= 191 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			ramMenu();
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 229 && cursor->y <= 272 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			storageMenu();
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

void processorMenu()
{	
	int currentState = stateOff;
	int cpufreq, cpu, bus;

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
		
		oslDrawString(20,76,"Current CPU Frequency");
		oslDrawString(20,128,"CPU Overclock");
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 118 && cursor->y <= 174)
		{
			oslDrawImageXY(highlight, 0, 122);
			oslDrawString(20,128,"CPU Overclock");
			oslDrawString(20,145,"Press R to increase frequency and L to decrease frequency");
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
			cpufreq = scePowerGetCpuClockFrequency();
			
			if (cpufreq <18)
			{
				scePowerSetClockFrequency(20, 20, 10);
			}
			else if (cpufreq <70)
			{
		        scePowerSetClockFrequency(75, 75, 37);	
			}
			else if (cpufreq <95)
			{
				scePowerSetClockFrequency(100, 100, 50);
			}
			else if (cpufreq <125)
			{
				scePowerSetClockFrequency(133, 133, 66);
			}
			else if (cpufreq <215)
			{
				scePowerSetClockFrequency(222, 222, 111);
			}
			else if (cpufreq <260)
			{
			scePowerSetClockFrequency(266, 266, 133);
			}
			else if (cpufreq <290)
			{
				scePowerSetClockFrequency(300, 300, 150);
			}
			else if (cpufreq <325)
			{
				scePowerSetClockFrequency(333, 333, 166);
			}
			
			currentState = stateNoClock;
		}
		
		if (currentState == stateUnderClock)
		{
			cpufreq = scePowerGetCpuClockFrequency();
			if (cpufreq> 300)
			{
				scePowerSetClockFrequency(300, 300, 150);
			}
			else if (cpufreq> 266)
			{
				scePowerSetClockFrequency(266, 266, 133);
			}
			else if (cpufreq> 222)
			{
				scePowerSetClockFrequency(222, 222, 111);
			}
			else if (cpufreq> 133)
			{
				scePowerSetClockFrequency(133, 133, 66);
			}
			else if (cpufreq> 100)
			{
				scePowerSetClockFrequency(100, 100, 50);
			}
			else if (cpufreq> 75)
		    {
				scePowerSetClockFrequency(75, 75, 37);
			}
			else if (cpufreq> 20)
			{
				scePowerSetClockFrequency(20, 20, 10);
			}
			
			currentState = stateNoClock; // This state allows the frequency to not climb on each cycle.
		}
		
		cpu = getCpuClock(); 
		bus = getBusClock(); 
		
		oslDrawStringf(20,87,"%d/%d",cpu, bus);
		oslDrawString(20,189,"Current CPU Frequency");
		oslDrawStringf(20,202,"%d MHz", cpu);
		oslDrawString(20,241,"Current BUS Frequency");
		oslDrawStringf(20,254,"%d MHz", bus);
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0);
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
		
		oslDrawStringf(20,98,"RAM Available: %d MB Available\n",oslGetRamStatus().maxAvailable/1000000); 
	
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0);
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
		
		oslDrawStringf(20,98,"Press Select to toggle USB mass storage"); 
		
		if (osl_keys->pressed.select)
		{
			enableUsb();
		}
		
		else if (osl_keys->pressed.select)
		{
			disableUsb();
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);

		digitaltime(381,4,0);
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
		
		oslDrawString(20,86,"Themes");
		oslDrawString(20,140,"Time");
		oslDrawString(20,195,"Miscellaneous");
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawString(20,86,"Themes");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 177)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawString(20,140,"Time");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 178 && cursor->y <= 228)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawString(20,195,"Miscellaneous");
		}

		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0);
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
		
		if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 62 && cursor->y <= 119 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);			
			displayThemes();
		}
		
		if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 179 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);			
			displayTime();
		}
		
		if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 178 && cursor->y <= 228 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);			
			displayMiscellaneous();
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
		
		oslDrawString(20,86,"Wallpaper");
		oslDrawString(20,140,"Fonts");
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawString(20,86,"Wallpaper");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 177)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawString(20,140,"Fonts");
		}

		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0);
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
		
		if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);			
			displaySubThemes(1);
		}
		
		if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 177 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);			
			displaySubThemes(0);
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
	digitaltime(381,4,0);
	
	// Displays the directories, while also incorporating the scrolling
	for(i=curScroll;i<MAX_SETTINGS_DISPLAY+curScroll;i++) {
	
		char * ext = strrchr(dirScan[i].name, '.'); //For file extension.
	
		// Handles the cursor and the display to not move past 4.
		// For moving down
		//if ((folderIcons[i].active == 0) && (current >= i-1)) {
	
		if ((folderIcons[i].active == 0) && (current >= i-1)) {
			current = i-1;
			break;
		}
		// For moving up
		if (current <= curScroll-1) {
			current = curScroll-1;
			break;
		}
		
		// If the currently selected item is active, then display the name
		if (folderIcons[i].active == 1) {
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
		FILE * fontPathTXT = fopen("system/fonts/fonts.txt", "w");
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

	if (pad.Buttons != oldpad.Buttons) {
	
		if ((pad.Buttons & PSP_CTRL_DOWN) && (!(oldpad.Buttons & PSP_CTRL_DOWN))) {
			settingsDown();
			timer = 0;
		}
		else if ((pad.Buttons & PSP_CTRL_UP) && (!(oldpad.Buttons & PSP_CTRL_UP))) {
			settingsUp();
			timer = 0;
		}
		
		if ((pad.Buttons & PSP_CTRL_RIGHT) && (!(oldpad.Buttons & PSP_CTRL_RIGHT))) {
			settingsDownx5();
			timer = 0;
		}
		else if ((pad.Buttons & PSP_CTRL_LEFT) && (!(oldpad.Buttons & PSP_CTRL_LEFT))) {
			settingsUpx5();	
			timer = 0;
		}
		
		if ((pad.Buttons & PSP_CTRL_TRIANGLE) && (!(oldpad.Buttons & PSP_CTRL_TRIANGLE))) {
			if (!(stricmp(lastDir, "ms0:")==0) || (stricmp(lastDir, "ms0:/")==0)) {
				curScroll = 1;
				current = 1;
			}
		}
		if ((pad.Buttons & PSP_CTRL_CROSS) && (!(oldpad.Buttons & PSP_CTRL_CROSS))) {
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

void displaySubThemes(int n) // n is used here to search for fonts or wallpaper
{	
	displaybg = oslLoadImageFilePNG("system/settings/displaybg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);

	if (!displaybg)
		debugDisplay();
		
	char * testDirectory;
		
	if (n == 0)	
		testDirectory = settingsBrowse("system/fonts", 0); //For fonts
	else if (n == 1)
		testDirectory = settingsBrowse("system/framework/framework-res/res", 1); //For wallpapers
	
	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		centerText(480/2, 272/2, testDirectory, 50);
		
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();	
	}
}

void displayTime()
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
		
		oslDrawString(20,86,"Use 24-hour format");
		oslDrawString(20,140,"Clock Style");
		oslDrawString(20,195,"Display AM/PM");
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawString(20,86,"Use 24-hour format");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 177)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawString(20,140,"Clock Style");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 178 && cursor->y <= 228)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawString(20,195,"Display AM/PM");
		}

		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0);
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

void displayMiscellaneous()
{	
	FILE * widgetActivation;
	FILE * eDesktopActivation;
	FILE * bootAnimActivation;
	FILE * gBootActivation;
	
	checkGBootActivation();
	
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
		
		oslDrawString(20,86,"Toggle day/night widget");
		oslDrawString(20,140,"Toggle expanded desktop");
		oslDrawString(20,195,"Toggle boot animation");
		oslDrawString(20,245,"Toggle game boot");
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 60 && cursor->y <= 117)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawString(20,86,"Toggle day/night widget");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 177)
		{
			oslDrawImageXY(highlight, 0, 118);
			oslDrawString(20,140,"Toggle expanded desktop");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 178 && cursor->y <= 227)
		{
			oslDrawImageXY(highlight, 0, 173);
			oslDrawString(20,195,"Toggle boot animation");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 228 && cursor->y <= 272)
		{
			oslDrawImageXY(highlight, 0, 228);
			oslDrawString(20,245,"Toggle game boot");
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();

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
		
		oslDrawStringf(20,83,"Password Lock"); 
		oslDrawStringf(20,144,"Pin Lock"); 
		
		if (cursor->x  >= 0 && cursor->x  <= 444 && cursor->y >= 61 && cursor->y <= 118)
		{	
			oslDrawImageXY(highlight, 0, 62);
			oslDrawStringf(20,83,"Password Lock"); 
			if (osl_keys->pressed.cross)
			{
				sceIoRemove("system/settings/password.bin");
				sceIoRemove("system/settings/pin.bin");
				openOSK("Enter Password", 20, -1);
				password = fopen("system/settings/password.bin", "w");
				fprintf(password, "%s", tempMessage);
				fclose(password);
			}
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 119 && cursor->y <= 178)
		{	
			oslDrawImageXY(highlight, 0, 120);
			oslDrawStringf(20,144,"Pin Lock"); 
			if (osl_keys->pressed.cross)
			{
				sceIoRemove("system/settings/password.bin");
				sceIoRemove("system/settings/pin.bin");
				openOSK("Enter Pin", 4, -1);
				pin = fopen("system/settings/pin.bin", "w");
				fprintf(pin, "%s", tempPin);
				fclose(pin);
			}
		}

		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);

		digitaltime(381,4,0);
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
        sprintf(Settings_message, "No WiFi configuration found");
        enabled = 0;
    }
	
	oslNetInit(); 
	
	if (!oslIsWlanPowerOn())
        sprintf(Settings_message, "Please turn on the Wlan switch.");

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
    		oslDrawString(10, wifi_y+28, configs[selectedConfig].name);
			oslDrawString(10, 206, "Press up or down to navigate through your Wifi configurations.");
        }
		
		oslDrawString(10, 220, Settings_message);
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		switchStatus(3);
		digitaltime(381,4,0);
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

	i = sceUmdCheckMedium();
	if(i == 0)
	{
		oslDrawString(4,5,"Insert UMD");
		i = sceUmdWaitDriveStat(PSP_UMD_PRESENT);
	}

	i = sceUmdActivate(1, "disc0:");
	oslDrawString(4,5,"Mounted disc");
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

		oslDrawStringf(4,15,"Found game %s\n", game_id);
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

	oslDrawString(4,5,"Dumping 28Megs from 0x8400000\n");
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

		oslDrawString(20,123,"Dump Flash 0");
		oslDrawString(20,181,"Dump Flash 1");
		oslDrawString(20,236,"More");
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 99 && cursor->y <= 155)
		{
			oslDrawImageXY(highlight, 0, 101);
			oslDrawString(20,123,"Dump Flash 0");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 156 && cursor->y <= 211)
		{
			oslDrawImageXY(highlight, 0, 158);
			oslDrawString(20,181,"Dump Flash 1");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 212 && cursor->y <= 268)
		{
			oslDrawImageXY(highlight, 0, 216);
			oslDrawString(20,236,"More");
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0);
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

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 99 && cursor->y <= 155 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			dump_filesystem("flash0:/", "ms0:/flash0");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 156 && cursor->y <= 211 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			dump_filesystem("flash1:/", "ms0:/flash1");
		}

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 212 && cursor->y <= 268 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			dumpMenuMore();
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

		oslDrawString(20,123,"Dump UMD boot.bin");
		oslDrawString(20,181,"Dump Memory");
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 99 && cursor->y <= 155)
		{
			oslDrawImageXY(highlight, 0, 101);
			oslDrawString(20,123,"Dump UMD boot.bin");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 156 && cursor->y <= 211)
		{
			oslDrawImageXY(highlight, 0, 158);
			oslDrawString(20,181,"Dump Memory");
		}

		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0);
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

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 99 && cursor->y <= 155 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			DumpBootBin();
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 156 && cursor->y <= 211 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			dump_memory();
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
		
		oslDrawString(10,107,"Toggle Remote Joy Lite");
		oslDrawString(10,121,"Displays your PSP screen on your computer via USB.");
		oslDrawString(10,135,"Press Triangle to disable or it may cause the program to crash");
		oslDrawString(10,168,"Toggle USB Debugging");
		oslDrawString(10,182,"Press Triangle to disable or it may cause the program to crash");
		oslDrawString(10,236,"Dumping Tools");
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 99 && cursor->y <= 154)
		{
			oslDrawImageXY(highlight, 0, 100);
			oslDrawString(10,107,"Toggle Remote Joy Lite");
			oslDrawString(10,121,"Displays your PSP screen on your computer via USB.");
			oslDrawString(10,135,"Press Triangle to disable or it may cause the program to crash");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 155 && cursor->y <= 214)
		{
			oslDrawImageXY(highlight, 0, 157);
			oslDrawString(10,168,"Toggle USB Debugging");
			oslDrawString(10,182,"Press Triangle to disable or it may cause the program to crash");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 215 && cursor->y <= 272)
		{
			oslDrawImageXY(highlight, 0, 216);
			oslDrawString(10,236,"Dumping Tools");
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0);
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
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 99 && cursor->y <= 154 && osl_keys->pressed.cross)
		{	
			oslPlaySound(KeypressStandard, 1);  
			RJL = 1;
			LoadStartModule("modules/RemoteJoyLite.prx");
		}
		if(RJL == 1 && osl_keys->pressed.triangle)
		{
			StopUnloadModule("modules/RemoteJoyLite.prx");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 155 && cursor->y <= 214 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			PSPDebug = 1;
			LoadStartModule("modules/psplink.prx");
		}
		if(PSPDebug == 1  && osl_keys->pressed.triangle)
		{	
			StopUnloadModule("modules/psplink.prx");			
		}

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 215 && cursor->y <= 272 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			dumpMenu();
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
		oslDrawString(50,120,"Wi-Fi");
	}
		
	if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 98 && cursor->y <= 154)
	{
		oslDrawImageXY(security, 222, 101);
		oslDrawString(280,120,"Security");
	}
	
	if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 155 && cursor->y <= 210)
	{
		oslDrawImageXY(themes, 0, 156);
		oslDrawString(50,177,"Display");
	}

	if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 155 && cursor->y <= 210)
	{
		oslDrawImageXY(performance, 225, 155);
		oslDrawString(280,177,"Performance");
	}
		
	if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 211 && cursor->y <= 267)
	{
		oslDrawImageXY(developeroptions, 0, 211);
		oslDrawString(50,234,"Developer Options");
	}

	if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 211 && cursor->y <= 267)
	{
		oslDrawImageXY(about, 226, 211);
		oslDrawString(280,233,"About");
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

		oslDrawString(50,177,"Display");
		oslDrawString(50,120,"Wi-Fi");
		oslDrawString(280,120,"Security");
		oslDrawString(50,234,"Developer Options");
		oslDrawString(280,177,"Performance");
		oslDrawString(280,234,"About");
		
		settingsHighlight();
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(381,4,0);
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