#include "settingsmenu.h"
#include "include/pgeZip.h"
#include "include/ram.h"
#include "include/utils.h"
#include "include/pspusbdevice.h"
#include "home.h"
#include "fm.h"
#include "clock.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "recoverymenu.h"
#include "screenshot.h"

char name;
int setclock;
char cyanogenpspversion[5] = "5.1";
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
					if (fileExists("ms0:/PSP/GAME/CyanogenPSP.zip"))
					{
					pgeZip* zipFiles = pgeZipOpen("../CyanogenPSP.zip");
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
					sceIoRemove("ms0:/PSP/GAME/CyanogenPSP/system/build.prop");
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
            int res = oslBrowserInit("ftp://31.170.163.207/CyanogenPSP.zip", "/PSP/GAME", 5*1024*1024,
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
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 121 && cursor->y <= 178)
		{
			oslDrawImageXY(highlight, 0, 122);
			pspGetModel(20,143);
			oslDrawStringf(20,129,"CyanogenPSP version: %s-20150308-OFFICIAL",cyanogenpspversion);
			oslDrawStringf(20,157,"Mac Address: %02X:%02X:%02X:%02X:%02X:%02X", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(386,4,424);
		battery(337,2,0);
		navbarButtons(2);
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
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
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			settingsMenu();	
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{	
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			home();	
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			multitask();
		}

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 62 && cursor->y <= 119 && osl_keys->pressed.cross)
		{
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			updatesMenu();
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 121 && cursor->y <= 178 && osl_keys->pressed.cross)
		{
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
			onlineUpdater();
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(386,4,424);
		battery(337,2,0);
		navbarButtons(2);
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
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
			oslDeleteImage(updatesbg);
			oslDeleteImage(highlight);
			aboutMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(updatesbg);
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
		
		digitaltime(386,4,424);
		battery(337,2,0);
		navbarButtons(2);
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
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
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			home();
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 75 && cursor->y <= 133 && osl_keys->pressed.cross)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			processorMenu();
		}

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 135 && cursor->y <= 191 && osl_keys->pressed.cross)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			ramMenu();
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 229 && cursor->y <= 272 && osl_keys->pressed.cross)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			storageMenu();
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
		
		digitaltime(386,4,424);
		battery(337,2,0);
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
			oslDeleteImage(processorbg);
			oslDeleteImage(highlight);
			performanceMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(processorbg);
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
		
		digitaltime(386,4,424);
		battery(337,2,0);
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
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			performanceMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
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

void storageMenu()
{	
	performancebg = oslLoadImageFilePNG("system/settings/performancebg2.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG("system/settings/offswitch.png", OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG("system/settings/onswitch.png", OSL_IN_RAM, OSL_PF_8888);

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
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_CENTER);
		
		if (osl_keys->pressed.select)
		{
			enableUsb();
		}
		
		else if (osl_keys->pressed.select)
		{
			disableUsb();
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);

		digitaltime(386,4,424);
		battery(337,2,0);
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
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			performanceMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{	
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			performanceMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
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

void displayMenu()
{	
	FILE * widgetActivation;
	FILE * eDesktopActivation;

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
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 62 && cursor->y <= 119)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawString(20,86,"Toggle day/night widget");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 179)
		{
			oslDrawImageXY(highlight, 0, 122);
			oslDrawString(20,140,"Toggle expanded desktop");
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
		digitaltime(386,4,424);
		battery(337,2,0);
		navbarButtons(2);
		androidQuickSettings();
		oslDrawImage(cursor);

		if (widgetActivator == 0)
		{
			oslDrawImageXY(offswitch,350,75);
			
			if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 62 && cursor->y <= 119 && osl_keys->pressed.cross)
			{
				widgetActivation = fopen("system/widget/widgetactivator.txt", "w");
				widgetActivator = 1;
				fprintf(widgetActivation, "1");
				fclose(widgetActivation);
			}
		}
		
		else if (widgetActivator == 1)
		{
			oslDrawImageXY(onswitch,350,75);
		
			if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 62 && cursor->y <= 119 && osl_keys->pressed.cross)
			{
				widgetActivation = fopen("system/widget/widgetactivator.txt", "w");
				widgetActivator = 0;
				fprintf(widgetActivation, "0");
				fclose(widgetActivation);
			}
		}
		
		if (eDesktopActivator == 0)
		{
			oslDrawImageXY(offswitch,350,128);
			
			if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 179 && osl_keys->pressed.cross)
			{
				eDesktopActivation = fopen("system/home/eDesktopActivator.txt", "w");
				eDesktopActivator = 1;
				fprintf(eDesktopActivation, "1");
				fclose(eDesktopActivation);
			}
		}
		
		else if (eDesktopActivator == 1)
		{
			oslDrawImageXY(onswitch,350,128);
		
			if(cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 118 && cursor->y <= 179 && osl_keys->pressed.cross)
			{
				eDesktopActivation = fopen("system/home/eDesktopActivator.txt", "w");
				eDesktopActivator = 0;
				fprintf(eDesktopActivation, "0");
				fclose(eDesktopActivation);
			}
		}
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			settingsMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
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
		digitaltime(386,4,424);
		battery(337,2,0);
		navbarButtons(2);
		androidQuickSettings();
		
		if (osl_keys->released.cross)
		{
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
			oslDeleteImage(wifibg);	
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(wifibg);	
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
		
		digitaltime(386,4,424);
		battery(337,2,0);
		navbarButtons(2);
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
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
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			developerMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			multitask();
		}

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 99 && cursor->y <= 155 && osl_keys->pressed.cross)
		{
			dump_filesystem("flash0:/", "ms0:/flash0");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 156 && cursor->y <= 211 && osl_keys->pressed.cross)
		{
			dump_filesystem("flash1:/", "ms0:/flash1");
		}

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 212 && cursor->y <= 268 && osl_keys->pressed.cross)
		{
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
		
		digitaltime(386,4,424);
		battery(337,2,0);
		navbarButtons(2);
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
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
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			dumpMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			home();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{	
			multitask();
		}

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 99 && cursor->y <= 155 && osl_keys->pressed.cross)
		{
			DumpBootBin();
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 156 && cursor->y <= 211 && osl_keys->pressed.cross)
		{
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
		
		digitaltime(386,4,424);
		battery(337,2,0);
		navbarButtons(2);
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
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
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(developerbg);
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
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 99 && cursor->y <= 154 && osl_keys->pressed.cross)
		{	
			RJL = 1;
			LoadStartModule("modules/RemoteJoyLite.prx");
		}
		if(RJL == 1 && osl_keys->pressed.triangle)
		{
			StopUnloadModule("modules/RemoteJoyLite.prx");
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 155 && cursor->y <= 214 && osl_keys->pressed.cross)
		{
			PSPDebug = 1;
			LoadStartModule("modules/psplink.prx");
		}
		if(PSPDebug == 1  && osl_keys->pressed.triangle)
		{	
			StopUnloadModule("modules/psplink.prx");			
		}

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 215 && cursor->y <= 272 && osl_keys->pressed.cross)
		{
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
		
		digitaltime(386,4,424);
		battery(337,2,0);
		navbarButtons(2);
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{	
			settingsDeleteResources();
			appdrawer();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{
			multitask();
		}
	
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			settingsDeleteResources();
			appdrawer();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			settingsDeleteResources();
			home();
		}
		
		if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 98 && cursor->y <= 154 && osl_keys->pressed.cross)
		{
			settingsDeleteResources();
			wifiMenu();
		}
		
		if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 155 && cursor->y <= 210 && osl_keys->pressed.cross)
		{
			settingsDeleteResources();
			displayMenu();
		}

		if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 155 && cursor->y <= 210 && osl_keys->pressed.cross)
		{
			settingsDeleteResources();
			performanceMenu();
		}
		
		if (cursor->x >= 3 && cursor->x <= 219 && cursor->y >= 211 && cursor->y <= 267 && osl_keys->pressed.cross)
		{
			settingsDeleteResources();
			developerMenu();
		}

		if (cursor->x >= 226 && cursor->x <= 442 && cursor->y >= 211 && cursor->y <= 267 && osl_keys->pressed.cross)
		{
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