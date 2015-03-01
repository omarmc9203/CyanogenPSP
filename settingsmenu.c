#include "settingsmenu.h"
#include "include/pgeZip.h"
#include "include/ram.h"
#include "include/utils.h"
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
char version[10] = "4.0";
char lang[12] = "Uk English";
static char Settings_message[100] = "";

const int cpu_list[] = { 20, 75, 100, 133, 166, 222, 266, 300, 333 };
const int bus_list[] = { 10, 37, 50, 66, 83, 111, 133, 150, 166 };
int current = 0;

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
			oslDrawImageXY(offswitch, 392, 68);
		
		else
			oslDrawImageXY(onswitch, 392, 68);
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
ro.build.date = Fri Feb 27 12:40 PM EST 2014",
	version, kuKernelGetModel(),lang,getCpuClock(),getBusClock());
	fclose(configtxt);	
}

void changer(int set) 
{
	switch (set) 
	{
		case 0:
		scePowerSetClockFrequency(cpu_list[set], cpu_list[set], bus_list[set]);
		break;
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

void pspGetCpuBus()
{	
	if(getCpuClock() == 20 && getBusClock() == 10)
	{
		oslDrawString(35,87,"20/10");
	}
   
	else if(getCpuClock() == 75 && getBusClock() == 37)
	{
		oslDrawString(35,87,"75/37");
	}
		
	else if(getCpuClock() == 100 && getBusClock() == 50)
	{
		oslDrawString(35,87,"100/50");
	}
		
	else if(getCpuClock() == 133 && getBusClock() == 66)
	{
		oslDrawString(35,87,"133/66");
	}
		
	else if(getCpuClock() == 166 && getBusClock() == 83)
	{
		oslDrawString(35,87,"166/83");
	}
		
	else if(getCpuClock() == 200 && getBusClock() == 100)
	{
		oslDrawString(35,87,"200/100");
	}
		
	else if(getCpuClock() == 222 && getBusClock() == 111)
	{
		oslDrawString(35,87,"222/111");
	}
		
	else if(getCpuClock() == 266 && getBusClock() == 133)
	{
		oslDrawString(35,87,"266/133");
	}
		
	else if(getCpuClock() == 300 && getBusClock() == 150)
	{
		oslDrawString(35,87,"300/150");
	}
		
	else if(getCpuClock() == 333 && getBusClock() == 166)
	{
		oslDrawString(35,87,"333/166");
	}
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
	
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
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
		oslDrawStringf(20,129,"CyanogenPSP version: 4.1-20150227-OFFICIAL");
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
			oslDrawStringf(20,129,"CyanogenPSP version: 4.1-20150227-OFFICIAL");
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
			oslDeleteFont(Roboto);
			settingsMenu();	
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{	
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			oslDeleteFont(Roboto);
			settingsMenu();	
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{	
			oslDeleteImage(aboutbg);
			oslDeleteImage(highlight);
			oslDeleteFont(Roboto);
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
			oslDeleteFont(Roboto);
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
				oslDeleteFont(Roboto);
				easterEgg();
			}
		}
		
		if(osl_keys->pressed.select)
		{
			enableUsb();
		}
		
		else if(osl_keys->pressed.select)
		{	
			disableUsb();
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
	
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
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
			oslDeleteFont(Roboto);
			aboutMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(updatesbg);
			oslDeleteImage(highlight);
			oslDeleteFont(Roboto);
			aboutMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(updatesbg);
			oslDeleteImage(highlight);
			oslDeleteFont(Roboto);
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
	
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
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
		oslDrawString(15,240,"Memory Management");
		
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
			oslDeleteFont(Roboto);
			settingsMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			oslDeleteFont(Roboto);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			oslDeleteFont(Roboto);
			home();
		}
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 75 && cursor->y <= 133 && osl_keys->pressed.cross)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			oslDeleteFont(Roboto);
			processorMenu();
		}

		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 135 && cursor->y <= 191 && osl_keys->pressed.cross)
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			oslDeleteFont(Roboto);
			ramMenu();
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

void wait_release(unsigned int buttons) 
{ 
    SceCtrlData pad; 
    sceCtrlReadBufferPositive(&pad, 1); 
	
    while (pad.Buttons & buttons) 
    { 
        sceKernelDelayThread(100000); 
        sceCtrlReadBufferPositive(&pad, 1); 
    } 
} 

unsigned int wait_press(unsigned int buttons) 
{ 
    SceCtrlData pad; 
	sceCtrlReadBufferPositive(&pad, 1); 
    
	while (1) 
    { 
        if (pad.Buttons & buttons) 
            return pad.Buttons & buttons; 
        sceKernelDelayThread(100000); 
        sceCtrlReadBufferPositive(&pad, 1); 
    } 
    return 0;   /* never reaches here, again, just to suppress warning */ 
} 

void set_cpu_clock(int n)
{
	if(n==0)
		scePowerSetClockFrequency(222,222,111);
	else if(n==1)
		scePowerSetClockFrequency(266,266,133);
	else if(n==2)
		scePowerSetClockFrequency(333,333,166);
}

void processorMenu()
{	
	int timer;

	processorbg = oslLoadImageFilePNG("system/settings/processorbg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslSetFont(Roboto);
	
	if (!processorbg || !highlight)
		debugDisplay();
	
	sceCtrlSetSamplingCycle(0); 
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL); 
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		sceCtrlPeekBufferPositive(&pad, 1);
		
		oslDrawImageXY(processorbg, 0, 0);
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_LEFT);
		
		oslDrawString(20,76,"Current CPU Frequency");
		oslDrawString(20,128,"CPU Overclock");
		oslDrawString(20,189,"Minimum CPU Frequency");
		oslDrawString(20,202,"20 MHz");
		oslDrawString(20,241,"Maximum CPU Frequency");
		oslDrawString(20,254,"333 MHz");
		
		if (cursor->x >= 16 && cursor->x <= 480 && cursor->y >= 118 && cursor->y <= 174)
		{
			oslDrawImageXY(highlight, 0, 121);
			oslDrawString(20,128,"CPU Overclock");
			oslDrawString(20,142,"Press R to increase frequency and L to decrease frequency");
			oslDrawString(20,156,"Press triangle to reset to default, 222/111");
		}

		if (osl_keys->pressed.triangle)
		{
		 setclock = 6;
		 scePowerSetClockFrequency(222, 222, 111);
		}
				
		if (current < 0)
		{
			current = MAX;
		}
		if (current > MAX)
		{
			current = 0;
		}
		
		if (osl_keys->pressed.L)
		{
		set_cpu_clock(1);
		}
		if (osl_keys->pressed.R)
		{
		set_cpu_clock(2);
		}
		
		oslDrawStringf(20,87,"%d/%d",cpu_list[current],bus_list[current]);
		
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
			oslDeleteFont(Roboto);
			performanceMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{	
			oslDeleteImage(processorbg);
			oslDeleteImage(highlight);
			oslDeleteFont(Roboto);
			performanceMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(processorbg);
			oslDeleteImage(highlight);
			oslDeleteFont(Roboto);
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
	
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslSetFont(Roboto);
	
	if (!performancebg || !highlight)
		debugDisplay();
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		sceCtrlPeekBufferPositive(&pad, 1);
		
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
			oslDeleteFont(Roboto);
			performanceMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{	
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			oslDeleteFont(Roboto);
			performanceMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(highlight);
			oslDeleteImage(performancebg);
			oslDeleteFont(Roboto);
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

	displaybg = oslLoadImageFilePNG("system/settings/displaybg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG("system/settings/offswitch.png", OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG("system/settings/onswitch.png", OSL_IN_RAM, OSL_PF_8888);
	
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
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
		
		oslDrawString(20,86,"Toggle Day/Night Widget");
		
		if (cursor->x >= 0 && cursor->x <= 444 && cursor->y >= 62 && cursor->y <= 119)
		{
			oslDrawImageXY(highlight, 0, 63);
			oslDrawString(20,86,"Toggle Day/Night Widget");
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
			oslDeleteFont(Roboto);
			settingsMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			oslDeleteFont(Roboto);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(displaybg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			oslDeleteFont(Roboto);
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
	
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslSetFont(Roboto);

	if (!wifibg)
		debugDisplay();
		
	int enabled = 1;
    int selectedConfig = 0;
	int wifi_y = 50;
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
    		oslDrawString(30, wifi_y+28, configs[selectedConfig].name);
			oslDrawString(30, 190, "Press up or down to navigate through your Wifi configurations.");
        }
		
		oslDrawString(30, 215, Settings_message);
		
		switchStatus(2);
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,INTRAFONT_ALIGN_LEFT);
		
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
			oslDeleteFont(Roboto);
			settingsMenu();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(wifibg);	
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			oslDeleteFont(Roboto);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(wifibg);	
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			oslDeleteFont(Roboto);
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
	
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
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
			oslDeleteFont(Roboto);
			developerMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			oslDeleteFont(Roboto);
			developerMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			oslDeleteFont(Roboto);
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
			oslDeleteFont(Roboto);
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
	
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
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
			oslDeleteFont(Roboto);
			dumpMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			oslDeleteFont(Roboto);
			dumpMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);
			oslDeleteFont(Roboto);
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
	int RJL;
	int PSPDebug;
	
	developerbg = oslLoadImageFilePNG("system/settings/developerbg.png", OSL_IN_RAM, OSL_PF_8888);
	highlight = oslLoadImageFilePNG("system/settings/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	offswitch = oslLoadImageFilePNG("system/settings/offswitch.png", OSL_IN_RAM, OSL_PF_8888);
	onswitch = oslLoadImageFilePNG("system/settings/onswitch.png", OSL_IN_RAM, OSL_PF_8888);
	
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
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
			oslDeleteFont(Roboto);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);	
			oslDeleteFont(Roboto);
			settingsMenu();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslDeleteImage(developerbg);
			oslDeleteImage(highlight);
			oslDeleteImage(offswitch);	
			oslDeleteImage(onswitch);	
			oslDeleteFont(Roboto);
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
			oslDeleteFont(Roboto);
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
	oslDeleteFont(Roboto);
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
		
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
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
		
		if (osl_keys->pressed.select)
		{
			enableUsb();
		}
		else if (osl_keys->pressed.select)
		{
			disableUsb();
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