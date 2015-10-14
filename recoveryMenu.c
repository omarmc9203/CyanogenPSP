// Thanks to West-Li, this program is based on his PSP Module Checker.
// Thanks DAX for his ipl_update.prx
// Thanks Yoti for his libpspident.a
// Thanks Raing3 for his psppower lib.
#include "recoveryMenu.h"
#include "modules/ipl_update.h"
#include "modules/batman.h"
#include "modules/kuman.h"
#include "include/common.h"
#include "include/utils.h"
#include "homeMenu.h"
#include "kuman_header.h"
#include "systemctrl_se.h"
#include "settingsMenu.h"

extern void CheckerPrintf(char *fmt, ...);

SceCtrlData pad;

SEConfig config;

void ShowPage5()
{
    int type, size, psp, pan, cfw;

	psp = chGetPSPHackable();
	pan = chGetPSPCreatePandora();
    cfw = chDetectCFW();
	
	chGetIplAttribute(&type, &size);
	sctrlSEGetConfig(&config);
	
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();	
		
		oslReadKeys();
	
		oslClearScreen(RGB(0,0,0));
		
		oslDrawImageXY(recoverybg, 0, 0);
		
		oslDrawStringf(10,60,"Advanced Configuration, CFW & PSP Utility Info");

		oslDrawStringf(10,80,"Plain modules in UMD/ISO     (currently: %s)\n", config.umdactivatedplaincheck ? "Enabled" : "Disabled");
		oslDrawStringf(10,90,"Execute PBOOT.BIN in UMD/ISO (currently: %s)\n", config.executebootbin ? "Enabled" : "Disabled");
		oslDrawStringf(10,100,"XMB Plugins                  (currently: %s)\n", config.xmbplugins ? "Disabled" : "Enabled");
		oslDrawStringf(10,110,"GAME Plugins                 (currently: %s)\n", config.gameplugins ? "Disabled" : "Enabled");
		oslDrawStringf(10,120,"POPS Plugins                 (currently: %s)\n\n", config.popsplugins ? "Disabled" : "Enabled");
		oslDrawStringf(10,130,"IPL:                %s (Size: 0x%08X)\n", IPLs[type], size);
		oslDrawStringf(10,140,"CFW:                %s\n\n", CFWs[cfw]);
		oslDrawStringf(10,150,"PSP Hackable:       %s\n", PSPUtils[psp]);
		oslDrawStringf(10,160,"PSP Create Pandora: %s\n\n\n", PSPUtils[pan]);

		oslDrawStringf(10,200,"Press L/R to switch sections, X to edit and Circle to return.\n");
		
		if (osl_keys->pressed.L)
		{
			oslDeleteImage(recoverybg);
			ShowPage4();
        }
		if (osl_keys->pressed.R)
		{
			oslDeleteImage(recoverybg);
			ShowPage1();
        }
		if (osl_keys->pressed.cross)
		{
			oslDeleteImage(recoverybg);
			ShowAdvancedCnfMenu();
        }
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(recoverybg);
			mainRecoveryMenu();
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
}

void ShowPage4()
{
	sctrlSEGetConfig(&config);
	
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();	
		
		oslReadKeys();
	
		oslClearScreen(RGB(0,0,0));
		
		oslDrawImageXY(recoverybg, 0, 0);
		
		oslDrawStringf(10,60,"Miscellaneous");
	
		oslDrawStringf(10,80,"Autoboot program at /PSP/GAME/BOOT/ (currently: %s)\n", config.startupprog ? "Enabled" : "Disabled");
		oslDrawStringf(10,90,"UMD Mode                       (currently: %s)\n", umdmodes[config.umdmode]);
		oslDrawStringf(10,100,"Fake region                    (currently: %s)\n", regions[config.fakeregion]);
		oslDrawStringf(10,110,"XMB Usb Device                 (currently: %s)\n", usbdevices[config.usbdevice]);
		oslDrawStringf(10,120,"Use network update             (currently: %s)\n", config.notusedaxupd ? "Disabled" : "Enabled");
		oslDrawStringf(10,130,"Hide PIC1.PNG and PIC0.PNG     (currently: %s)\n", config.hidepics ? "Enabled" : "Disabled");
		oslDrawStringf(10,140,"Use version.txt                (currently: %s)\n", config.useversiontxt ? "Enabled" : "Disabled");
		oslDrawStringf(10,150,"Use Slim colors on Classic PSP (currently: %s)\n", config.slimcolors ? "Enabled" : "Disabled");
		oslDrawStringf(10,160,"Hide MAC address               (currently: %s)\n\n\n", config.hidemac ? "Enabled" : "Disabled");
	
		oslDrawStringf(10,200,"Press L/R to switch sections, X to edit and Circle to return.\n");
		
		if (osl_keys->pressed.L)
		{
			oslDeleteImage(recoverybg);
			ShowPage3();
        }
		if (osl_keys->pressed.R)
		{
			oslDeleteImage(recoverybg);
			ShowPage5();
        }
		if (osl_keys->pressed.cross)
		{
			oslDeleteImage(recoverybg);
			ShowCnfMenu();
        }
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(recoverybg);
			mainRecoveryMenu();
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
}

void backupPassword()
{	
	u32 pass;

	char password = GetRegistryValue("/CONFIG/SYSTEM/LOCK", "password", &pass);
	
	FILE * configtxt = fopen(PWD, "wb"); //create config file
	fprintf(configtxt, "Password: %d\n", password);
	fclose(configtxt);	
}

//Based on raing3s CheckSysInfoP4 
void ShowPage3()
{
    int cpu, bus;
	char button[256], wma[256], flash[256];
	u8 kirk[4], spock[4], macaddr[512];
	u32 pass;
	
	*(u32 *)kirk = chGetKirk();
	*(u32 *)spock = chGetSpock();
	
	u32 value;
	
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();	
		
		oslReadKeys();
	
		oslClearScreen(RGB(0,0,0));
		
		oslDrawImageXY(recoverybg, 0, 0);
	
		GetRegistryValue("/CONFIG/SYSTEM/XMB", "button_assign", &value);

		if (value == 1) 
		{
			oslDrawStringf(10,160,button, "Swap buttons: Yes (currently: O is enter)\n");		
		} 
		else 
		{
			oslDrawStringf(10,160,button, "Swap buttons: No (currently: X is enter)\n");		
		}
	
		GetRegistryValue("/CONFIG/MUSIC", "wma_play", &value);

		if (value == 1)
		{
			oslDrawStringf(10,170,wma, "WMA:          Yes\n");	
		}
		else
		{
			oslDrawStringf(10,170,wma, "WMA:          No\n");		
		}
	
		GetRegistryValue("/CONFIG/BROWSER", "flash_activated", &value);

		if (value == 1)
		{
			oslDrawStringf(10,180,flash, "Flash player: Yes\n\n\n");	
		}
		else
		{
			oslDrawStringf(10,180,flash, "Flash player: No\n\n\n");		
		}
		
		cpu = getCpuClock(); 
		bus = getBusClock(); 
		chGetMACAddress(macaddr);

		char password = GetRegistryValue("/CONFIG/SYSTEM/LOCK", "password", &pass);
	
		oslDrawStringf(10,60,"System Information");

		oslDrawStringf(10,80,"CPU Speed:    %i/%i Mhz\n", cpu, bus);
		oslDrawStringf(10,90,"WLAN:         %s\n\n", sceWlanGetSwitchState() == 0 ? "Off" : "On");
		oslDrawStringf(10,100,"Name:         %s\n", pspname);
		oslDrawStringf(10,110,"Password:     %d\n", password);
	
		if(VerifyFile("machide.chk") == -1)
		{
			oslDrawStringf(10,120,"MAC Address:  %02X:%02X:%02X:%02X:%02X:%02X\n\n", macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
		}
		else
		{
			oslDrawStringf(10,120,"MAC Address:  00:00:00:00:00:00 (Hidden)\n\n");   
		}
	   
		oslDrawStringf(10,130,"Scramble:     0x%08X\n\n", chGetScramble());
		oslDrawStringf(10,140,"Kirk:         %c%c%c%c\n", kirk[3], kirk[2], kirk[1], kirk[0]);
	
		if(kuKernelGetModel() == 4)
		{
			oslDrawStringf(10,150,"Spock:        -\n\n");
		}
		else
		{
			oslDrawStringf(10,150,"Spock:        %c%c%c%c\n\n", spock[3], spock[2], spock[1], spock[0]); 
		}
	
		oslDrawStringf(10,200,"Press L/R to switch sections, X to edit and Circle to return.\n");
		
		if (osl_keys->pressed.L)
		{
			oslDeleteImage(recoverybg);
			ShowPage2();
        }
		if (osl_keys->pressed.R)
		{
			oslDeleteImage(recoverybg);
			ShowPage4();
        }
		if (osl_keys->pressed.cross)
		{
			oslDeleteImage(recoverybg);
			ShowSystemMenu();
        }
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(recoverybg);
			mainRecoveryMenu();
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
}

//Based on raing3s CheckSysInfoP2
void ShowPage2()
{
    int batser, batmode;
	
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();	
		
		oslReadKeys();
		
		oslClearScreen(RGB(0,0,0));
		
		oslDrawImageXY(recoverybg, 0, 0);
	
		oslDrawStringf(10,60,"Battery Information");
	
		batmode = GetBatType();
		batser = GetBatSer();

		oslDrawStringf(10,80,"Battery Status:  %s\n", scePowerIsBatteryCharging() == 1 ? "Charging" : "Using");
		oslDrawStringf(10,90,"Battery %%:       %i%%\n\n", scePowerGetBatteryLifePercent() < 0 ? 0 : scePowerGetBatteryLifePercent());
		oslDrawStringf(10,100,"Hours Left:   %i:%02i\n\n", scePowerGetBatteryLifeTime() < 0 ? 0 : (scePowerGetBatteryLifeTime() / 60), scePowerGetBatteryLifeTime() < 0 ? 0 : (scePowerGetBatteryLifeTime() - (scePowerGetBatteryLifeTime() / 60 * 60)));
		oslDrawStringf(10,110,"Battery Temp:    %iºC\n", scePowerGetBatteryTemp() < 0 ? 0 : scePowerGetBatteryTemp());
		oslDrawStringf(10,120,"Battery Voltage: %0.3fV\n\n", scePowerGetBatteryVolt() < 0 ? 0 : (float)scePowerGetBatteryVolt() / 1000.0);
		oslDrawStringf(10,130,"Remain Capacity: %i mAh\n", scePowerGetBatteryRemainCapacity() < 0 ? 0 : scePowerGetBatteryRemainCapacity()); //From raing3s psppower
	
		if(chGetPSPCreatePandora() == 0) //Yes so we can show the serial, mode of battery
		{
			oslDrawStringf(10,140,"Battery Type:    %s\n", Batterys[batmode]);
			oslDrawStringf(10,150,"Battery Serial:  0x%08X\n\n\n", batser);
		}
		else
		{
			oslDrawStringf(10,140,"Battery Type:    -\n");
			oslDrawStringf(10,150,"Battery Serial:  -\n\n\n");
		}
	
		oslDrawStringf(10,200,"Press L/R to switch sections, X to edit and Circle to return.\n");

		if (osl_keys->pressed.L)
		{
			oslDeleteImage(recoverybg);
			ShowPage1();
        }
		if (osl_keys->pressed.R)
		{
			oslDeleteImage(recoverybg);
			ShowPage3();
        }
		if (osl_keys->pressed.cross)
		{
			oslDeleteImage(recoverybg);
			ShowBatteryMenu();
        }
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(recoverybg);
			mainRecoveryMenu();
		}
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
}

fw_version getFwVersion(fw_version *v)
{
		long int a = sceKernelDevkitVersion();
		v->major = (*((char *)&a+3));
		v->minor = (*((char *)&a+2)*10) + (*((char *)&a+1));
		return *v;
}

void ShowPage1()
{
    int baryon, pommel, tachyon, fuseid, fusecfg, mb, model, type, region;

	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	
	fw_version version;
	getFwVersion(&version);
	
	while (!osl_quit)
	{
		LowMemExit();

		oslStartDrawing();	
		
		oslReadKeys();
		
		oslClearScreen(RGB(0,0,0));
		
		oslDrawImageXY(recoverybg, 0, 0);
	
		oslDrawStringf(10,60,"PSP General Information");
		
		chGetVersions(&baryon, &pommel, &tachyon, &fuseid, &fusecfg);
	
		mb = chDetectMotherboard();
		model = chDetectModel();	
		type = chDetectOFW();
		region = chGetRegion();		
		
		oslDrawStringf(10,80,"Kernel Version: %d.%d (0x%08X)\n\n", version.major, version.minor, sceKernelDevkitVersion());
		pspGetModel(10,90);
		oslDrawStringf(10,100,"Module:         %s\n", Modules[model]);      
		oslDrawStringf(10,110,"Motherboard:    %s\n\n", MBs[mb]);
		oslDrawStringf(10,120,"Tachyon:        0x%08X\n", tachyon);
		oslDrawStringf(10,130,"Baryon:         0x%08X\n", baryon);
		oslDrawStringf(10,140,"Pommel:         0x%08X\n\n", pommel);
		oslDrawStringf(10,150,"Fuse ID:        0x%08X\n", fuseid);
		oslDrawStringf(10,160,"Fuse CFG:       0x%08X\n\n", fusecfg);
		oslDrawStringf(10,170,"Region:         %s\n\n\n", Regions[region]);
	
		oslDrawStringf(10,200,"Press L/R to switch sections and Circle to return.\n");
		
		if (osl_keys->pressed.L)
		{
			oslDeleteImage(recoverybg);
			ShowPage5();
        }
		if (osl_keys->pressed.R)
		{
			oslDeleteImage(recoverybg);
			ShowPage2();
        }
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(recoverybg);
			mainRecoveryMenu();
		}
	   
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
}

void ShowVersionTxt()
{
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();	
	
		oslReadKeys();
	
		oslClearScreen(RGB(0,0,0));
		
		oslDrawImageXY(recoverybg, 0, 0);
	
		oslDrawStringf(10,60,"PSP Version.txt");
	
		if(CheckVersion(Version) == -1)
			oslDrawStringf(10,80,"Version.txt not found...");
	     
		oslDrawStringf(10,80,"Version.txt:\n");
		oslDrawStringf(10,90," \n%s\n\n", GetVersion());	
	
		oslDrawStringf(10,200,"Press Circle to return to Main Menu.\n\n");

		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(recoverybg);
		    mainRecoveryMenu();
		}
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
}

int ConfigurationMenu()
{
	int MenuSelection = 1; // Pretty obvious
	int selector_x = 0; //The x position of the first selection
	int selector_y = 35; //The y position of the first selection
	int selector_image_x = 0; //Determines the starting x position of the selection
	int selector_image_y = 0; //Determines the starting y position of the selection
	int numMenuItems = 5; //Amount of items in the menu
		
	int selection = 0;
	
	sctrlSEGetConfig(&config);
	
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	Selector = oslLoadImageFile("android_bootable_recovery/res/images/selector.png", OSL_IN_RAM, OSL_PF_8888);
	
	while (!osl_quit)
	{
		LowMemExit();

		oslStartDrawing();	
	
		oslReadKeys();
		
		oslClearScreen(RGB(0,0,0));
			
		oslDrawImageXY(recoverybg, 0, 0);
		oslDrawImage(Selector);
		
		oslDrawStringf(10,60,"Go back");
		oslDrawStringf(10,80,"Advanced configuration");
		oslDrawStringf(10,100,"General configuration");
		oslDrawStringf(10,120,"System");
		oslDrawStringf(10,140,"Battery");
	
		Selector->x = selector_image_x; //Sets the selection coordinates
        Selector->y = selector_image_y; //Sets the selection coordinates
        
        selector_image_x = selector_x+(selector_xDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(selector_yDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last
		
		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {
			unloadRecoveryMenuRes();
			mainRecoveryMenu();
        }
		
		else if (MenuSelection == 2 && osl_keys->pressed.cross)
        {
			unloadRecoveryMenuRes();
			ShowAdvancedCnfMenu();
        }
	
		else if (MenuSelection == 3 && osl_keys->pressed.cross)
        {
			unloadRecoveryMenuRes();
			ShowCnfMenu();
        }
		
		else if (MenuSelection == 4 && osl_keys->pressed.cross)
        {
			unloadRecoveryMenuRes();
			ShowSystemMenu();
        }
		
		else if (MenuSelection == 5 && osl_keys->pressed.cross)
        {
			unloadRecoveryMenuRes();
			ShowBatteryMenu();
        }
		
		if (osl_keys->pressed.circle)
		{
			unloadRecoveryMenuRes();
		    mainRecoveryMenu();
		}	
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
	return selection;
}

int ShowAdvancedCnfMenu()
{	
	
	int MenuSelection = 1; // Pretty obvious
	int selector_x = 0; //The x position of the first selection
	int selector_y = 35; //The y position of the first selection
	int selector_image_x = 0; //Determines the starting x position of the selection
	int selector_image_y = 0; //Determines the starting y position of the selection
	int numMenuItems = 6; //Amount of items in the menu

	int selection = 0;
	
	sctrlSEGetConfig(&config);
	
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	Selector = oslLoadImageFile("android_bootable_recovery/res/images/selector.png", OSL_IN_RAM, OSL_PF_8888);
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();	
		
		oslReadKeys();
		
		oslClearScreen(RGB(0,0,0));
			
		oslDrawImageXY(recoverybg, 0, 0);
		oslDrawImage(Selector);

		oslDrawStringf(10,60,"Go back");
		oslDrawStringf(10,80,"Plain modules in UMD/ISO     (currently: %s)", config.umdactivatedplaincheck ? "enabled" : "disabled");
		oslDrawStringf(10,100,"Execute PBOOT.BIN in UMD/ISO (currently: %s)", config.executebootbin ? "enabled" : "disabled");
		oslDrawStringf(10,120,"XMB plugins                  (currently: %s)", config.xmbplugins ? "disabled" : "enabled");
		oslDrawStringf(10,140,"GAME plugins                 (currently: %s)", config.gameplugins ? "disabled" : "enabled");
		oslDrawStringf(10,160,"POPS plugins                 (currently: %s)", config.popsplugins ? "disabled" : "enabled");
	
		Selector->x = selector_image_x; //Sets the selection coordinates
        Selector->y = selector_image_y; //Sets the selection coordinates
        
        selector_image_x = selector_x+(selector_xDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(selector_yDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last

		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {
			unloadRecoveryMenuRes();
		    ConfigurationMenu();
        }	
		else if (MenuSelection == 2 && osl_keys->pressed.cross)
        {
			config.umdactivatedplaincheck = !config.umdactivatedplaincheck;
			sctrlSESetConfig(&config);
			unloadRecoveryMenuRes();
			ShowAdvancedCnfMenu();
        }
        else if (MenuSelection == 3 && osl_keys->pressed.cross)
        {
			config.executebootbin = !config.executebootbin;
			sctrlSESetConfig(&config);
			unloadRecoveryMenuRes();
			ShowAdvancedCnfMenu();
        }	
        else if (MenuSelection == 4 && osl_keys->pressed.cross)
        {
			config.xmbplugins = !config.xmbplugins;
			sctrlSESetConfig(&config);
			unloadRecoveryMenuRes();
			ShowAdvancedCnfMenu();
        }	
        else if (MenuSelection == 5 && osl_keys->pressed.cross)
        {
			config.gameplugins = !config.gameplugins;
			sctrlSESetConfig(&config);
			unloadRecoveryMenuRes();
			ShowAdvancedCnfMenu();
        }
        else if (MenuSelection == 6 && osl_keys->pressed.cross)
        {
			config.popsplugins = !config.popsplugins;
			sctrlSESetConfig(&config);
			unloadRecoveryMenuRes();
			ShowAdvancedCnfMenu();
        }
		
		if (osl_keys->pressed.circle)
		{
			unloadRecoveryMenuRes();
		    ConfigurationMenu();
		}		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
	return selection;
}

int ShowCnfMenu()
{
    int MenuSelection = 1; // Pretty obvious
	int selector_x = 0; //The x position of the first selection
	int selector_y = 35; //The y position of the first selection
	int selector_image_x = 0; //Determines the starting x position of the selection
	int selector_image_y = 0; //Determines the starting y position of the selection
	int numMenuItems = 10; //Amount of items in the menu
		
	int selection = 0;
	
	sctrlSEGetConfig(&config);
	
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	Selector = oslLoadImageFile("android_bootable_recovery/res/images/selector.png", OSL_IN_RAM, OSL_PF_8888);
	
	while (!osl_quit)
	{
		LowMemExit();
  
		oslStartDrawing();	
		
		oslReadKeys();
	
		oslClearScreen(RGB(0,0,0));
		
		oslDrawImageXY(recoverybg, 0, 0);
		oslDrawImage(Selector);

		oslDrawStringf(10,60,"Go back");
		oslDrawStringf(10,80,"Autoboot program at /PSP/GAME/BOOT/ (currently: %s)", config.startupprog ? "Enabled" : "Disabled");
		oslDrawStringf(10,100,"UMD Mode                       (currently: %s)", umdmodes[config.umdmode]);
		oslDrawStringf(10,120,"Fake region                    (currently: %s)", regions[config.fakeregion]);
		oslDrawStringf(10,140,"XMB Usb Device                 (currently: %s)", usbdevices[config.usbdevice]);
		oslDrawStringf(10,160,"Use network update             (currently: %s)", config.notusedaxupd ? "Disabled" : "Enabled");
		oslDrawStringf(10,180,"Hide PIC1.PNG and PIC0.PNG     (currently: %s)", config.hidepics ? "Enabled" : "Disabled");
		oslDrawStringf(10,200,"Use version.txt                (currently: %s)", config.useversiontxt ? "Enabled" : "Disabled");
		oslDrawStringf(10,220,"Use Slim colors on Classic PSP (currently: %s)", config.slimcolors ? "Enabled" : "Disabled");
		oslDrawStringf(10,240,"Hide MAC address               (currently: %s)", config.hidemac ? "Enabled" : "Disabled");

		Selector->x = selector_image_x; //Sets the selection coordinates
        Selector->y = selector_image_y; //Sets the selection coordinates
        
        selector_image_x = selector_x+(selector_xDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(selector_yDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last

		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {
			unloadRecoveryMenuRes();
		    ConfigurationMenu();
        }
		else if (MenuSelection == 2 && osl_keys->pressed.cross)
        {
			config.startupprog = !config.startupprog;
			sctrlSESetConfig(&config);
			unloadRecoveryMenuRes();
			ShowCnfMenu();
        }
        else if (MenuSelection == 3 && osl_keys->pressed.cross)
        {
			if(config.umdmode == MODE_UMD)
                config.umdmode = MODE_MARCH33;
            else if(config.umdmode == MODE_MARCH33)
                config.umdmode = MODE_NP9660;
            else if(config.umdmode == MODE_NP9660)
                config.umdmode = MODE_OE_LEGACY;
            else if(config.umdmode == MODE_OE_LEGACY)
                config.umdmode = MODE_UMD;
			sctrlSESetConfig(&config);
			unloadRecoveryMenuRes();
			ShowCnfMenu();
        }
        else if (MenuSelection == 4 && osl_keys->pressed.cross)
        {
			if(config.fakeregion == FAKE_REGION_DISABLED)
                config.fakeregion = FAKE_REGION_JAPAN;
            else if(config.fakeregion == FAKE_REGION_JAPAN)
                config.fakeregion = FAKE_REGION_AMERICA;
            else if(config.fakeregion == FAKE_REGION_AMERICA)
                config.fakeregion = FAKE_REGION_EUROPE;
            else if(config.fakeregion == FAKE_REGION_EUROPE)
                config.fakeregion = FAKE_REGION_KOREA;
            else if(config.fakeregion == FAKE_REGION_KOREA)
                config.fakeregion = FAKE_REGION_UNK;
            else if(config.fakeregion == FAKE_REGION_UNK)
                config.fakeregion = FAKE_REGION_UNK2;
            else if(config.fakeregion == FAKE_REGION_UNK2)
                config.fakeregion = FAKE_REGION_AUSTRALIA;
            else if(config.fakeregion == FAKE_REGION_AUSTRALIA)
                config.fakeregion = FAKE_REGION_HONGKONG;
            else if(config.fakeregion == FAKE_REGION_HONGKONG)
                config.fakeregion = FAKE_REGION_TAIWAN;
            else if(config.fakeregion == FAKE_REGION_TAIWAN)
                config.fakeregion = FAKE_REGION_RUSSIA;
            else if(config.fakeregion == FAKE_REGION_RUSSIA)
                config.fakeregion = FAKE_REGION_CHINA;
            else if(config.fakeregion == FAKE_REGION_CHINA)
                config.fakeregion = FAKE_REGION_DEBUG_TYPE_I;
            else if(config.fakeregion == FAKE_REGION_DEBUG_TYPE_I)
                config.fakeregion = FAKE_REGION_DEBUG_TYPE_II;
            else if(config.fakeregion == FAKE_REGION_DEBUG_TYPE_II)
                config.fakeregion = FAKE_REGION_DISABLED;
			sctrlSESetConfig(&config);
			unloadRecoveryMenuRes();
			ShowCnfMenu();
        }
        else if (MenuSelection == 5 && osl_keys->pressed.cross)
        {
			if(config.usbdevice == USBDEVICE_MEMORYSTICK)
                config.usbdevice = USBDEVICE_FLASH0;
            else if(config.usbdevice == USBDEVICE_FLASH0)
                config.usbdevice = USBDEVICE_FLASH1;
            else if(config.usbdevice == USBDEVICE_FLASH1)
                config.usbdevice = USBDEVICE_FLASH2;
            else if(config.usbdevice == USBDEVICE_FLASH2)
                config.usbdevice = USBDEVICE_FLASH3;
            else if(config.usbdevice == USBDEVICE_FLASH3)
                config.usbdevice = USBDEVICE_UMD9660;
            else if(config.usbdevice == USBDEVICE_UMD9660)
                config.usbdevice = USBDEVICE_MEMORYSTICK;
				
			sctrlSESetConfig(&config);
			unloadRecoveryMenuRes();
			ShowCnfMenu();
        } 
        else if (MenuSelection == 6 && osl_keys->pressed.cross)
        {
			config.notusedaxupd = !config.notusedaxupd;
			sctrlSESetConfig(&config);
			unloadRecoveryMenuRes();
			ShowCnfMenu();
        }
        else if (MenuSelection == 7 && osl_keys->pressed.cross)
        {
			config.hidepics = !config.hidepics;
			sctrlSESetConfig(&config);
			unloadRecoveryMenuRes();
			ShowCnfMenu();
        } 
        else if (MenuSelection == 8 && osl_keys->pressed.cross)
        {
			config.useversiontxt = !config.useversiontxt;
			sctrlSESetConfig(&config);
			unloadRecoveryMenuRes();
			ShowCnfMenu();
        } 
        else if (MenuSelection == 9 && osl_keys->pressed.cross)
        {
			config.slimcolors = !config.slimcolors;
			sctrlSESetConfig(&config);
			unloadRecoveryMenuRes();
			ShowCnfMenu();
        } 
        else if (MenuSelection == 10 && osl_keys->pressed.cross)
        {
			config.hidemac = !config.hidemac;
			sctrlSESetConfig(&config);
			unloadRecoveryMenuRes();
			ShowCnfMenu();
        }

		if (osl_keys->pressed.circle)
		{
			unloadRecoveryMenuRes();
		    ConfigurationMenu();
		}			
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
	return selection;
}

int ShowSystemMenu()
{
    int MenuSelection = 1; // Pretty obvious
	int selector_x = 0; //The x position of the first selection
	int selector_y = 35; //The y position of the first selection
	int numMenuItems = 6; //Amount of items in the menu
	int selector_image_x = 0; //Determines the starting x position of the selection
	int selector_image_y = 0; //Determines the starting y position of the selection

	int selection = 0;
	
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	Selector = oslLoadImageFile("android_bootable_recovery/res/images/selector.png", OSL_IN_RAM, OSL_PF_8888);
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();	
		
		oslReadKeys();
	
		oslClearScreen(RGB(0,0,0));
		
		oslDrawImageXY(recoverybg, 0, 0);
		oslDrawImage(Selector);
	
		oslDrawStringf(10,60,"Go back");
		oslDrawStringf(10,80,"Swap buttons");
		oslDrawStringf(10,100,"Activate WMA");
		oslDrawStringf(10,120,"Activate flash player");
		oslDrawStringf(10,140,"Use fake name");
		oslDrawStringf(10,160,"Hide MAC address");
	
		Selector->x = selector_image_x; //Sets the selection coordinates
        Selector->y = selector_image_y; //Sets the selection coordinates
        
        selector_image_x = selector_x+(selector_xDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(selector_yDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last

		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {
			unloadRecoveryMenuRes();
		    ConfigurationMenu();
        } 
		else if (MenuSelection == 2 && osl_keys->pressed.cross)
        {
		    oslDrawStringf(10,200,"Assigning...");
			oslSyncFrame();
			sceKernelDelayThread(2*1000000);
			swap_buttons();
			unloadRecoveryMenuRes();
			ShowSystemMenu();
        }
		else if (MenuSelection == 3 && osl_keys->pressed.cross)
        {
		    oslDrawStringf(10,200,"Activating...");
			oslSyncFrame();
			sceKernelDelayThread(2*1000000);
			active_wma();
			unloadRecoveryMenuRes();
			ShowSystemMenu();
        }
		else if (MenuSelection == 4 && osl_keys->pressed.cross)
        {
		    oslDrawStringf(10,200,"Activating...");
			oslSyncFrame();
			sceKernelDelayThread(2*1000000);
			active_flash();
			unloadRecoveryMenuRes();
			ShowSystemMenu();
        }
		else if (MenuSelection == 5 && osl_keys->pressed.cross)
        {
		    oslDrawStringf(10,200,"Faking...");
			oslSyncFrame();
			sceKernelDelayThread(2*1000000);
			fake_name();
			unloadRecoveryMenuRes();
			ShowSystemMenu();
        }
		else if (MenuSelection == 6 && osl_keys->pressed.cross)
        {
		    oslDrawStringf(10,200,"Hiding...");
			oslSyncFrame();
			sceKernelDelayThread(2*1000000);
			hide_mac();
			unloadRecoveryMenuRes();
			ShowSystemMenu();
        }
		
		if (osl_keys->pressed.circle)
		{
			unloadRecoveryMenuRes();
		    ConfigurationMenu();
		}		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
	return selection;
}

int ShowBatteryMenu()
{
	char BatSerCurrent[4];
	
	int MenuSelection = 1; // Pretty obvious
	int selector_x = 0; //The x position of the first selection
	int selector_y = 35; //The y position of the first selection
	int selector_image_x = 0; //Determines the starting x position of the selection
	int selector_image_y = 0; //Determines the starting y position of the selection
	int numMenuItems = 5; //Amount of items in the menu
		
	int selection = 0;
	
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	Selector = oslLoadImageFile("android_bootable_recovery/res/images/selector.png", OSL_IN_RAM, OSL_PF_8888);
	
    
	while (!osl_quit)
	{
		LowMemExit();

		oslStartDrawing();	
		
		oslReadKeys();
	
		oslClearScreen(RGB(0,0,0));
		
		oslDrawImageXY(recoverybg, 0, 0);
		oslDrawImage(Selector);
	
		oslDrawStringf(10,60,"Go back");
		oslDrawStringf(10,80,"Make battery pandora");
		oslDrawStringf(10,100,"Make battery autoBoot");
		oslDrawStringf(10,120,"Make battery normal");
		oslDrawStringf(10,140,"Dump battery serial to file");
	
		Selector->x = selector_image_x; //Sets the selection coordinates
        Selector->y = selector_image_y; //Sets the selection coordinates
        
        selector_image_x = selector_x+(selector_xDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(selector_yDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last

		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {
			unloadRecoveryMenuRes();
		    ConfigurationMenu();
        } 
		else if (MenuSelection == 2 && osl_keys->pressed.cross)
        {			
			if(chGetPSPCreatePandora() == 1) oslDrawStringf(10,190,"Your hardware is not supported.");
			
			oslDrawStringf(10,200,"Creating Pandora Battery...");
			oslSyncFrame();
			sceKernelDelayThread(2*1000000);
			SetBatSer(0xFFFF, 0xFFFF);
	        oslDrawStringf(10,210,"Done");
			oslSyncFrame();
			sceKernelDelayThread(1*1000000);
			unloadRecoveryMenuRes();
			ShowBatteryMenu();
        }
		else if (MenuSelection == 3 && osl_keys->pressed.cross)
        {
			if(chGetPSPCreatePandora() == 1) oslDrawStringf(10,190,"Your hardware is not supported.");
			
			oslDrawStringf(10,200,"Creating AutoBoot Battery...");
			oslSyncFrame();
			sceKernelDelayThread(2*1000000);
			SetBatSer(0x0000, 0x0000);
			oslDrawStringf(10,210,"Done");
			oslSyncFrame();
			sceKernelDelayThread(1*1000000);
			unloadRecoveryMenuRes();
			ShowBatteryMenu();
        }
		else if (MenuSelection == 4 && osl_keys->pressed.cross)
        {			
			if(chGetPSPCreatePandora() == 1) oslDrawStringf(10,190,"Your hardware is not supported.");
			
			oslDrawStringf(10,210,"Creating Normal Battery...");
			oslSyncFrame();
			sceKernelDelayThread(2*1000000);
			SetBatSer(0x5241, 0x4E44);
			oslDrawStringf(10,200,"Done");
			oslSyncFrame();
			sceKernelDelayThread(1*1000000);
			unloadRecoveryMenuRes();
			ShowBatteryMenu();
        }
		else if (MenuSelection == 5 && osl_keys->pressed.cross)
        {
			if(chGetPSPCreatePandora() == 1) 
			{	
				oslDrawStringf(10,190,"Your hardware is not supported.");
				oslSyncFrame();
				sceKernelDelayThread(3*1000000);
			}
			oslDrawStringf(10,210,"Dumping Battery Serial...");
			oslSyncFrame();
			sceKernelDelayThread(2*1000000);
			GetBatSer(BatSerCurrent);
			WriteFile("ms0:/batser.bin", BatSerCurrent, 4);
			oslDrawStringf(10,200,"Done");
			oslSyncFrame();
			sceKernelDelayThread(1*1000000);
			unloadRecoveryMenuRes();
			ShowBatteryMenu();
        }
		
		if (osl_keys->pressed.circle)
		{
			unloadRecoveryMenuRes();
		    ConfigurationMenu();
		}	
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
	return selection;
}

void unloadRecoveryMenuRes()
{
	oslDeleteImage(recoverybg);
	oslDeleteImage(Selector);
}

int mainRecoveryMenu()
{
	int MenuSelection = 1; // Pretty obvious
	int selector_x = 0; //The x position of the first selection
	int selector_y = 35; //The y position of the first selection
	int numMenuItems = 9; //Amount of items in the menu
	int selector_image_x = 0; //Determines the starting x position of the selection
	int selector_image_y = 0; //Determines the starting y position of the selection
	
	int selection = 0;
	
	recoverybg = oslLoadImageFilePNG("android_bootable_recovery/res/images/recoverybg.png", OSL_IN_RAM, OSL_PF_8888);
	Selector = oslLoadImageFile("android_bootable_recovery/res/images/selector.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!recoverybg || !Selector)
		debugDisplay();
	
	roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslIntraFontSetStyle(roboto, 0.5f, RGBA(100,192,231,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	oslSetFont(roboto);

	while (!osl_quit)
	{
		LowMemExit();
  
		oslStartDrawing();	

		oslReadKeys();
		
		oslClearScreen(RGB(0,0,0));
		
		oslDrawImageXY(recoverybg, 0, 0);
		oslDrawImage(Selector);
		
		oslDrawStringf(10,60,"Toggle USB");
		oslDrawStringf(10,80,"System information");
		oslDrawStringf(10,100,"Configuration");
        oslDrawStringf(10,120,"Show version.txt");
		oslDrawStringf(10,140,"Plugins");
		oslDrawStringf(10,160,"Exit");
		oslDrawStringf(10,180,"Exit to XMB");
		oslDrawStringf(10,200,"Standby");
		oslDrawStringf(10,220,"Shutdown device");
		
		Selector->x = selector_image_x; //Sets the selection coordinates
        Selector->y = selector_image_y; //Sets the selection coordinates
        
        selector_image_x = selector_x+(selector_xDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(selector_yDistance*MenuSelection); //Determines where the selection image is drawn for each selection
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last
		
		if (MenuSelection == 1 && osl_keys->pressed.cross)
		{
			enableUsb();
		}
		
		else if (MenuSelection == 1 && osl_keys->pressed.cross)
		{
			disableUsb();
		}
		
		else if (MenuSelection == 2 && osl_keys->pressed.cross)
		{
			unloadRecoveryMenuRes();
			ShowPage1();
		}
		
		else if (MenuSelection == 3 && osl_keys->pressed.cross)
		{
			unloadRecoveryMenuRes();
			ConfigurationMenu();
		}
		
		else if (MenuSelection == 4 && osl_keys->pressed.cross)
		{
			unloadRecoveryMenuRes();
			ShowVersionTxt();
		}
		
		else if (MenuSelection == 6 && osl_keys->pressed.cross)
		{
			unloadRecoveryMenuRes();
			oslDeleteFont(roboto);
			oslSyncFrame();
			sceKernelDelayThread(3*1000000);
			home();
		}
		
		else if (MenuSelection == 7 && osl_keys->pressed.cross)
		{
			xmbExit();
		}
		
		else if (MenuSelection == 8 && osl_keys->pressed.cross)
		{
			deviceStandby();
		}
		
		else if (MenuSelection == 9 && osl_keys->pressed.cross)
		{
			shutdownDevice();
		}
		
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();
	}
	return selection;
}
