#include "home.h"
#include "language.h"
#include "appdrawer.h"
#include "apollo.h"
#include "clock.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "settingsmenu.h"
#include "screenshot.h"
#include "include/utils.h"

//variables
int result;
int notif_y = -272;
int yPos1 = -272;
int yPos2 = -272;
int yLine1 = -272;
int yLine2 = -272;
int controlX = 25;

int getBrightness(void);
void setBrightness(int brightness);
int displayEnable(void);
int displayDisable(void);

int imposeGetVolume();
int imposeSetVolume();

void internet() //Draws the browser
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
					home();
                }
            }
            oslEndDrawing();
		}
		oslEndFrame();
		skip = oslSyncFrame();

        if (!browser)
		{
            oslReadKeys();
            oslBrowserInit("http://www.google.com", "/PSP/GAME/CyanogenPSP/downloads", 5*1024*1024, //Downloads will be saved into this directory
                                         PSP_UTILITY_HTMLVIEWER_DISPLAYMODE_SMART_FIT,
                                         PSP_UTILITY_HTMLVIEWER_DISABLE_STARTUP_LIMITS,
                                         PSP_UTILITY_HTMLVIEWER_INTERFACEMODE_FULL,
                                         PSP_UTILITY_HTMLVIEWER_CONNECTMODE_MANUAL_ALL);
			memset(message, 0, sizeof(message));

        }
    }
	oslNetTerm();
}

void debugDisplay()
{
	debug = oslLoadImageFilePNG("system/debug/debug.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	while (!osl_quit)
	{	
		oslStartDrawing();

		controls();	
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,0);

		oslDrawImageXY(debug, 65, 67);
		oslDrawStringf(110,95,"Unfortunately some files required by");
		oslDrawStringf(110,110,"the program are missing. The program");
		oslDrawStringf(110,125,"has stopped, press (X) to exit to XMB.");

		if (osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			sceKernelExitGame();
		}
	
		if (osl_pad.held.R && osl_keys->pressed.triangle) //Takes screenshot
		{
			screenshot();
		}
	
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();
	}
}

void controls() //Main controller function - allows cursor movement
{
	//Intialize the limits
	int llimit = 0;
	int rlimit = 460;
	int ulimit = 0;
	int dlimit = 252;
		
	//Read keys
	oslReadKeys();
		
	if(osl_keys->analogX >= 50)
		cursor->x+= osl_keys->analogX/30;
		
	if(osl_keys->analogX <= -50)
		cursor->x+=osl_keys->analogX/30;

	if(osl_keys->analogY >= 50)
		cursor->y+= osl_keys->analogY/30;
		
	if(osl_keys->analogY <= -50)
		cursor->y+= osl_keys->analogY/30;
		
	/* Prevents the cursor from going off screen */
	
	if (cursor->x <= llimit)
	{
		cursor->x = llimit;
	}
		
	else if (cursor->x >= rlimit)
	{
		cursor->x = rlimit;
	}
		
	if (cursor->y <= ulimit)
	{	
		cursor->y = ulimit;
	}
		
	else if (cursor->y >= dlimit)
	{
		cursor->y = dlimit;
	}
}

void battery(int batx, int baty, int n) // Draws the battery icon depending on its percentage. 
{
	int batteryLife, y = 0;
	
	batteryLife = scePowerGetBatteryLifePercent(); //Gets battery percentage
	
	if (n == 0 || n == 1)
	{
		if (batteryLife <= 99)
		{
			oslDrawStringf(batx+18, 4,"%d%%",batteryLife);
		}
		else  if (batteryLife > 99)
		{
			oslDrawStringf(batx+16, 4,"%d%%",batteryLife);
		}
		
		if (usbStat == 1)
		{
			oslDrawImageXY(usbdebug,5, -1);
		}
			
		else if (isPlaying == 1)
		{
			oslDrawImageXY(music,5, 2);
		}
		
		if (usbStat == 1 && isPlaying == 1)
		{
			oslDrawImageXY(usbdebug, 5, -1);
			oslDrawImageXY(music, 26, 2);
		}
	}
	
	if (n == 3)
	{
		if ((cursor->y <= 16) || (cursor->y >= 226))
		{
			oslDrawImageXY(layerA, 0, 0);
			y+=2;
		}
		
		else if (cursor->y >= 16 && cursor->y <= 226)
		{
			y-=17;
		}
		
		if (batteryLife <= 99)
		{
			baty = y;
			oslDrawStringf(batx+18, y+2,"%d%%",batteryLife);
		}
		else  if (batteryLife > 99)
		{
			baty = y;
			oslDrawStringf(batx+16, y+2,"%d%%",batteryLife);
		}
		
		if (usbStat == 1)
		{
			oslDrawImageXY(usbdebug,5, y-3);
		}
			
		else if (isPlaying == 1)
		{
			oslDrawImageXY(music,5, y);
		}
		
		if (usbStat == 1 && isPlaying == 1)
		{
			oslDrawImageXY(usbdebug, 5, y-3);
			oslDrawImageXY(music, 26, y);
		}
	}
	
	if (batteryLife == 100)
		oslDrawImageXY(batt100,batx,baty);
	else if (batteryLife >80 && batteryLife <= 100) 
		oslDrawImageXY(batt80,batx,baty);
	else if (batteryLife >60 && batteryLife <= 80)
		oslDrawImageXY(batt60,batx,baty);
	else if (batteryLife >40 && batteryLife <= 60)
		oslDrawImageXY(batt40,batx,baty);
	else if (batteryLife >20 && batteryLife <= 40) 
		oslDrawImageXY(batt20,batx,baty);
	else if (batteryLife >10 && batteryLife <= 20)
		oslDrawImageXY(batt10,batx,baty);
	else if (batteryLife >0 && batteryLife <= 10)
	{
		//oslPlaySound(LowBattery, 1);  
		oslDrawImageXY(batt0,batx,baty);
	}
	
	if (scePowerIsBatteryCharging() == 1) // If the battery's charging, draw the charging icon on the battery icon.
		//oslPlaySound(WirelessChargingStarted, 1);  
		oslDrawImageXY(battcharge,batx,baty);
}

//Imports
void increase_volume(int n);
void decrease_volume(int n);

void volumeController()
{	
	int llimit = 150;
	int rlimit = 350;

	int controlX = 0;
	int volume = imposeGetVolume();
	
	if (volume == 0)
	{
		controlX = 150;
	}
	else if (volume == 3)
	{
		controlX = 170;
	}
	else if (volume == 6)
	{
		controlX = 190;
	}
	else if (volume == 9)
	{
		controlX = 210;
	}
	else if (volume == 12)
	{
		controlX = 230;
	}
	else if (volume == 15)
	{
		controlX = 250;
	}
	else if (volume == 18)
	{
		controlX = 270;
	}
	else if (volume == 21)
	{
		controlX = 290;
	}
	else if (volume == 24)
	{
		controlX = 310;
	}
	else if (volume == 27)
	{
		controlX = 330;
	}
	else if (volume == 30)
	{
		controlX = 350;
	}
	if (osl_keys->pressed.up)
	{
		controlX += 3;
		increase_volume(1);
	}
	if (osl_keys->pressed.down)
	{
		controlX -= 3;
		decrease_volume(1);
	}
	
	if (controlX <= llimit)
	{
		controlX = llimit;
	}
	else if (controlX >= rlimit)
	{
		controlX = rlimit;
	}
	
	if (osl_pad.held.up)
	{
		oslDrawImageXY(volumeBar, 97,30);
		oslDrawImageXY(volumeControl, controlX, 54);
		oslDrawStringf(115,75, "Vol: %d", imposeGetVolume());
		oslSyncFrame();
		sceKernelDelayThread(1*1000000);
	}
	
	if (osl_pad.held.down)
	{
		oslDrawImageXY(volumeBar, 97,30);
		oslDrawImageXY(volumeControl, controlX, 54);
		oslDrawStringf(115,75, "Vol: %d", imposeGetVolume());
		oslSyncFrame();
		sceKernelDelayThread(1*1000000);
	}
}

void appDrawerIcon() //Draws the app drawer icon. Draws a different icon of the same size once hovered with the cursor.
{
	if (cursor->x  >= 215 && cursor->x  <= 243 && cursor->y >= 195 && cursor->y <= 230)
		oslDrawImageXY(ic_allapps_pressed,218,197);
	
	else
		oslDrawImageXY(ic_allapps,218,197);
}

void navbarButtons(int n) //Draws the navbar buttons in the bottom as seen on androids.
{		
	int y = 272, ulimit = 237, dlimit = 272;

	if (n == 0)
	{
		oslDrawImageXY(navbar, 109, 237);
		
		if (cursor->x  >= 144 && cursor->x  <= 204 && cursor->y >= 226 && cursor->y <= 271)
			oslDrawImageXY(backicon,109, 237); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon, else it just draws the navbar.
	
		else
			oslDrawImageXY(navbar,109, 237);
		
		if (cursor->x  >= 205 && cursor->x  <= 271 && cursor->y >= 226 && cursor->y <= 271)
			oslDrawImageXY(homeicon,109, 237); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon, else it just draws the navbar.
	
		else
			oslDrawImageXY(navbar,109, 237);
		
		if (cursor->x  >= 272 && cursor->x  <= 332 && cursor->y >= 226 && cursor->y <= 271)
			oslDrawImageXY(multicon,109, 237); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon. else it just draws the navbar.
	
		else
			oslDrawImageXY(navbar,109, 237);
	}
	
	if (n == 1)
	{
		if ((cursor->y >= 226) || (cursor->y <= 16) )
		{
			oslDrawImageXY(layerB, 0, 237);
			y-=35;
			
			if (y <= ulimit)
			{	
				y = ulimit;
			}
	
			oslDrawImageXY(navbar, 109, y);
		
			if (cursor->x  >= 144 && cursor->x  <= 204 && cursor->y >= 226 && cursor->y <= 271)
				oslDrawImageXY(backicon,109, y); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon, else it just draws the navbar.
	
			else
				oslDrawImageXY(navbar,109, y);
		
			if (cursor->x  >= 205 && cursor->x  <= 271 && cursor->y >= 226 && cursor->y <= 271)
				oslDrawImageXY(homeicon,109, y); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon, else it just draws the navbar.
	
			else
				oslDrawImageXY(navbar,109, y);
		
			if (cursor->x  >= 272 && cursor->x  <= 332 && cursor->y >= 226 && cursor->y <= 271)
				oslDrawImageXY(multicon,109, y); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon. else it just draws the navbar.
	
			else
				oslDrawImageXY(navbar,109, y);
		}
		else if (cursor->y >= 16 && cursor->y <= 226)
		{
			y+=35;
			
			if (y >= dlimit)
			{	
				y = dlimit;
			}
		}
	}
	
	if (n == 2)
	{
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75))
		{
			oslDrawImageXY(multicon2, 444, 19); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon, else it just draws the navbar.
		}
	
		else if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213))
		{
			oslDrawImageXY(backicon2, 444, 19); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon. else it just draws the navbar.
		}

		else if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155))
		{
			oslDrawImageXY(homeicon2, 444, 20); //If the cursor is moved onto/near the back icon, it displays the highlighted back icon, else it just draws the navbar.
		}
		else
		{
			oslDrawImageXY(navbar2, 444, 19);
		}
	}
}

void androidQuickSettings()
{
	int notif_up;
	int notif_down;
	int notif_enabled;
	
	int llimit = 20;
	int rlimit = 420;
	
	oslDrawImageXY(quickSettings,0,notif_y);
	
	battery(374,yPos2-2,2);
	getDayOfWeek(15,yPos2+5,1);
	getMonthOfYear(88,yPos2+5);

	oslDrawStringf(137,yLine1, "%s", lang_quickSettings[language][0]);
	oslDrawStringf(335,yPos2, "%d%%",scePowerGetBatteryLifePercent());
	oslDrawStringf(415,yPos2, "%.7s", pspname);
	
	int cpu = getCpuClock();
	
	if (cpu <= 133)
	{
		oslDrawStringf(285,yLine1, "%s", lang_quickSettings[language][1]);
	}
	else if (cpu >= 300)
	{
		oslDrawStringf(284,yLine1, "%s", lang_quickSettings[language][3]);
	}
	else 
	{
		oslDrawStringf(292,yLine1, "%s", lang_quickSettings[language][2]);
	}
		
	oslDrawStringf(202,yLine2, "%s", lang_quickSettings[language][4]);
	
	oslDrawImageXY(control,controlX,yPos2+66);	
		
	digitaltime(15,yPos1,53,hrTime);
	
	if (getBrightness() == 10)
	{
		controlX = 20;
	}
	else if (getBrightness() == 20)
	{
		controlX = 70;
	}
	else if (getBrightness() == 30)
	{
		controlX = 120;
	}
	else if (getBrightness() == 40)
	{
		controlX = 170;
	}
	else if (getBrightness() == 50)
	{
		controlX = 220;
	}
	else if (getBrightness() == 60)
	{
		controlX = 270;
	}
	else if (getBrightness() == 70)
	{
		controlX = 320;
	}
	else if (getBrightness() == 80)
	{
		controlX = 370;
	}
	else if (getBrightness() == 90)
	{
		controlX = 420;
	}
		
	//This was in while (1) {}
	int amt = getBrightness();
	
	if (amt < 10) 
	{
		setBrightness(10);
	}

	if (amt > 90) 
	{
		setBrightness(90);
	}
	
	if (controlX <= llimit)
	{
		controlX = llimit;
	}
	else if (controlX >= rlimit)
	{
		controlX = rlimit;
	}
	
	notif_enabled = 0;
	
	if (osl_pad.held.cross && cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 0 && cursor->y <= 1) 
	{
		notif_down = 1;
	}

	else if (osl_pad.held.cross && cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 250 && notif_y == 0)
	{
		notif_up = 1;
	}
			
	if (notif_down == 1)
	{				
		if (osl_pad.held.cross && osl_keys->analogY >= 50)
		{
			notif_y = notif_y+6;
			yPos1 = yPos1+6;
			yPos2 = yPos2+6;
			yLine1 = yLine1+6;
			yLine2 = yLine2+6;
		}
		
		if (notif_y >= 0)
		{
			notif_y = 0;
		}
		
		if (yPos1 >= 10)
		{
			yPos1 = 10;
		}
		if (yPos2 >= 20)
		{
			yPos2 = 20;
		}
		if (yLine1 >= 180)
		{
			yLine1 = 180;
		}
		if (yLine2 >= 245)
		{
			yLine2 = 245;
		}
		if (yLine2 >= 200)
		{
			notif_enabled = 1;
		}
	}
	
	if (notif_enabled == 1)
	{
		if (osl_keys->held.right)
		{
			controlX += 4;
			setBrightness(getBrightness() + 1);
		}
		if (osl_keys->held.left)
		{
			controlX -= 4;
			setBrightness(getBrightness() - 1);
		}	
		
		if (cursor->x >= 386 && cursor->x <= 414 && cursor->y >= 12 && cursor->y <= 38 && osl_keys->pressed.cross)
		{	
			oslPlaySound(KeypressStandard, 1);  
			notif_y = notif_y-272;
			yPos1 = yPos1-272;
			yPos2 = yPos2-272;
			yLine1 = yLine1-272;
			yLine2 = yLine2-272;
			settingsMenu();
		}
	
		if (cursor->x >= 96 && cursor->x <= 206 && cursor->y >= 126 && cursor->y <= 192 && osl_keys->pressed.cross)
		{	
			oslPlaySound(KeypressStandard, 1);  
			notif_y = notif_y-272;
			yPos1 = yPos1-272;
			yPos2 = yPos2-272;
			yLine1 = yLine1-272;
			yLine2 = yLine2-272;
			wifiMenu();
		}
		
		if (cursor->x >= 198 && cursor->x <= 272 && cursor->y >= 204 && cursor->y <= 258 && osl_keys->pressed.cross)
		{
			oslPlaySound(Lock, 1);
			notif_y = notif_y-272;
			yPos1 = yPos1-272;
			yPos2 = yPos2-272;
			yLine1 = yLine1-272;
			yLine2 = yLine2-272;
			lockscreen();
		}
	}
	if (notif_down == 1 && osl_keys->pressed.circle)
	{
		notif_y = notif_y-272;
		yPos1 = yPos1-272;
		yPos2 = yPos2-272;
		yLine1 = yLine1-272;
		yLine2 = yLine2-272;
	}
	
	if (notif_up == 1)
	{		
		notif_enabled = 0;
		
		if (osl_pad.held.cross && osl_keys->analogY <= -50)
		{
			notif_y = notif_y-6;
			yPos1 = yPos1-6;
			yPos2 = yPos2-6;
			yLine1 = yLine1-6;
			yLine2 = yLine2-6;
		}
		
		if (notif_y <= -272)
		{	
			notif_y = -272;
			yPos1 = -272;
			yPos2 = -272;
			yLine1 = -272;
			yLine2 = -272;
		}
	}
}

void notif_2()
{
	while(!osl_quit)
	{
		oslStartDrawing();
		oslCopyImageTo(notif,notif2);	
		oslEndDrawing();
        oslEndFrame(); 
		oslSyncFrame();	
	}
}

void loadIcons() // Loading the app drawer icons.
{
	ic_allapps = oslLoadImageFilePNG(allappsPath, OSL_IN_RAM, OSL_PF_8888);
	ic_allapps_pressed = oslLoadImageFile(allapps_pressedPath, OSL_IN_RAM, OSL_PF_8888);
}

void unloadIcons() //Deleting the app drawer icons to save memory.
{
	oslDeleteImage(ic_allapps);
	oslDeleteImage(ic_allapps_pressed);
}

void LowMemExit() //This is temporary until I come up with a solution. It exits the app, once the memory is less than/equal to 1.5 MB
{
	if (oslGetRamStatus().maxAvailable <= 1500000)
	{
		oslQuit();
	}
}

void dayNightCycleWidget()
{
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);
	
	if (hrTime == 0)
	{
		if (time.hour > 12)
			time.hour -= 12;
	
		if (time.hour == 00)
			time.hour = 12;
	}
	
    oslIntraFontSetStyle(Roboto, 1.7f,WHITE,0,INTRAFONT_ALIGN_CENTER);
	oslDrawStringf(235,70,"%2d:%02d", time.hour, time.minutes);
		
	oslIntraFontSetStyle(Roboto, 0.4f,WHITE,0,INTRAFONT_ALIGN_CENTER);
	getMonthOfYear(302,102);
	getDayOfWeek(196,102,2);
	
	if (time.hour < 6)
	oslDrawImageXY(wDay, 210, 82);
	
	else
	oslDrawImageXY(wNight, 205, 82);
}

void checkWidgetActivation()
{
	FILE * widgetActivation;

	if (!(fileExists("system/widget/widgetactivator.txt")))
	{
		widgetActivation = fopen("system/widget/widgetactivator.txt", "w");
		fprintf(widgetActivation, "1");
		fclose(widgetActivation);
	}

	widgetActivation = fopen("system/widget/widgetactivator.txt", "r");
	fscanf(widgetActivation,"%d",&widgetActivator);
	fclose(widgetActivation);
}

void checkEDesktopActivation()
{
	FILE * eDesktopActivation;

	if (!(fileExists("system/home/eDesktopActivator.txt")))
	{
		eDesktopActivation = fopen("system/home/eDesktopActivator.txt", "w");
		fprintf(eDesktopActivation, "0");
		fclose(eDesktopActivation);
	}

	eDesktopActivation = fopen("system/home/eDesktopActivator.txt", "r");
	fscanf(eDesktopActivation,"%d",&eDesktopActivator);
	fclose(eDesktopActivation);
}

void homeUnloadResources()
{
	oslDeleteImage(ic_allapps);
	oslDeleteImage(ic_allapps_pressed);
	oslDeleteImage(wDay);
	oslDeleteImage(wNight);
}

void home()
{	
	loadIcons();
	wDay = oslLoadImageFilePNG("system/widget/Day.png", OSL_IN_RAM, OSL_PF_8888);
	wNight = oslLoadImageFile("system/widget/Night.png", OSL_IN_RAM, OSL_PF_8888);
	
	oslSetFont(Roboto);
	
	checkWidgetActivation();
	checkEDesktopActivation();
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();

		controls();	
		
		oslDrawImage(background);		
		oslDrawImageXY(ic_launcher_apollo, 105, 190);
		oslDrawImageXY(ic_launcher_browser, 276, 190);
		oslDrawImageXY(ic_launcher_settings, 331, 190);
		oslDrawImageXY(ic_launcher_messenger, 160, 190);
		oslDrawImageXY(pointer, 232, 180);
		
		appDrawerIcon();
		
		if (widgetActivator == 1)
		dayNightCycleWidget();
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
		
		if (eDesktopActivator == 1)
		{
			navbarButtons(1);
			battery(370,2,3);
			if ((cursor->y <= 16) || (cursor->y >= 226))
			{
				digitaltime(420,4,0,hrTime);
			}
			else if (cursor->y >= 16 && cursor->y <= 226)
			{
				digitaltime(420,-10,0,hrTime);
			}	
		}
		else if (eDesktopActivator == 0)
		{
			digitaltime(420,4,0,hrTime);
			navbarButtons(0);
			battery(370,2,1);
		}
		
		androidQuickSettings();
		//volumeController();
		appHighlight(0);
		oslDrawImage(cursor);
			
		if (osl_keys->pressed.square)
		{
			powermenu();
		}

		if (cursor->x >= 276 && cursor->x <= 321 && cursor->y >= 195 && cursor->y <= 240 && osl_keys->pressed.cross)
		{
			homeUnloadResources();
			internet();
		}
		
		if (cursor->x >= 330 && cursor->x <= 374 && cursor->y >= 190 && cursor->y <= 240 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			homeUnloadResources();
			settingsMenu();
		}
		
		if (cursor->x >= 100 && cursor->x <= 154 && cursor->y >= 195 && cursor->y <= 240 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			homeUnloadResources();
			mp3player();
		}
		
		/*
		if (cursor->x >= 155 && cursor->x <= 210 && cursor->y >= 195 && cursor->y <= 240 && osl_keys->pressed.cross)
		{
			homeUnloadResources();
			messenger();
		}
		*/
			
		if (cursor->x >= 215 && cursor->x <= 243 && cursor->y >= 195 && cursor->y <= 230 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			homeUnloadResources();
			appdrawer();
		}

		if (osl_keys->pressed.L)
		{	
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			oslPlaySound(KeypressStandard, 1);  
			screenshot();
		}
	
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
}

