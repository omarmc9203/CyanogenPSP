#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspsdk.h>
#include <pspctrl.h>
#include <pspsysmem.h>
#include <string.h>
#include <malloc.h> 

//OSLib
#include <oslib/oslib.h>

//PSP Time
#include <psprtc.h>

//Parts of the Shell
#include "apollo.h"
#include "appdrawer.h"
#include "calculator.h"
#include "clock.h"
#include "fm.h"
#include "game.h"
#include "home.h"
#include "lock.h"
#include "main.h"
#include "multi.h"
#include "mp3player.h"
#include "power_menu.h"
#include "recoverymenu.h"
#include "settingsmenu.h"

#include "screenshot.h"
#include "include/ram.h"

#define ADDRESS "www.google.com"

PSP_MODULE_INFO("CyanogenPSP",  1, 4, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU); 
PSP_HEAP_SIZE_KB(24000);

//declaration
OSL_IMAGE *background, *cursor, *ic_allapps, *ic_allapps_pressed, *navbar, *wificon, *music, *gmail, *messengericon, *browser, *google, *notif, *batt100, 
		  *batt80, *batt60, *batt40, *batt20, *batt10, *batt0, *battcharge, *pointer, *pointer1, *backicon, *homeicon, *multicon, *usbdebug, *quickSettings,
		  *recoverybg, *welcome, *transbackground, *notif2 , *wifibg, *playing, *debug, *brightness, *control, *navbar2, *backicon2, *homeicon2, *multicon2;
		  
//definition of our sounds
OSL_SOUND *tone;
OSL_FONT *DroidSans;

static int runningFlag = 1;
static char message[100] = "";
static char buffer[100] = "";

int scePower_0442D852(int unknown);

int initOSLib() //Intialize OsLib
{
    oslInit(0);
    oslInitGfx(OSL_PF_8888, 1);
	oslSetBilinearFilter(1);
    oslInitAudio();
    oslSetKeyAutorepeatInit(40);
    oslSetKeyAutorepeatInterval(10);
    return 0;
}

// Functions imported from prx:

int imposeGetVolume();
int imposeSetVolume();
int imposeGetMute();
int imposeSetMute(int value);

int getBrightness(void);
void setBrightness(int brightness);

int imposeSetBrightness(int value);
int imposeGetBrightness();
int displayEnable(void);
int displayDisable(void);
int getBrightness();
void setBrightness(int brightness);
int readButtons(SceCtrlData *pad_data, int count);
int imposeSetHomePopup(int value);

void initSystemButtons();
unsigned int readSystemButtons(void);
unsigned int readHomeButton(void);
unsigned int readVolumeButtons(void);
unsigned int readVolUpButton(void);
unsigned int readVolDownButton(void);
unsigned int readNoteButton(void);
unsigned int readScreenButton(void);
unsigned int readHoldSwitch(void);
unsigned int readWLANSwitch(void);
int readMainVolume(void);
 
/* Exit callback */
int exit_callback(int arg1, int arg2, void *common)
 {
	sceKernelExitGame();
	return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp) 
{
	int cbid;
	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void) 
{
	int thid = 0;
	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, THREAD_ATTR_USER, 0);
	if(thid >= 0) sceKernelStartThread(thid, 0, 0);
	return thid;
}

int connectAPCallback(int state) //Internet stuff
{
    oslStartDrawing();
    oslDrawImageXY(wifibg, 0, 19);
    oslDrawString(30, 175, "Connecting to AP...");
    sprintf(buffer, "State: %i", state);
    oslDrawString(30, 195, buffer);
    oslEndDrawing();
    oslEndFrame();
    oslSyncFrame();

    return 0;
} 
 
int connectToAP(int config) //Internet stuff
{
    oslStartDrawing();
    oslDrawImageXY(wifibg, 0, 19);
    oslDrawString(30, 175, "Connecting to AP...");
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
        oslDrawString(30, 175, buffer);

        sprintf(buffer, "Resolving %s", ADDRESS);
        oslDrawString(30, 195, buffer);
        oslEndDrawing();
        oslEndFrame();
        oslSyncFrame();

        result = oslResolveAddress(ADDRESS, resolvedIP);

        oslStartDrawing();
        oslDrawImageXY(wifibg, 0, 19);
        oslGetIPaddress(ip);
        if (!result)
            sprintf(buffer, "Resolved IP address: %s", ip);
        else
            sprintf(buffer, "Error resolving address!");
        oslDrawString(30, 195, buffer);
        oslEndDrawing();
        oslEndFrame();
        oslSyncFrame();
		sceKernelDelayThread(3*1000000);
    }else{
        oslStartDrawing();
        oslDrawImageXY(wifibg, 0, 19);
        sprintf(buffer, "Error connecting to AP!");
        oslDrawString(30, 195, buffer);
        oslEndDrawing();
        oslEndFrame();
        oslSyncFrame();
		sceKernelDelayThread(3*1000000);
    }
    oslDisconnectFromAP();
    return 0;
} 

void debugDisplay()
{
	debug = oslLoadImageFilePNG("system/debug/debug.png", OSL_IN_RAM, OSL_PF_8888);
	transbackground = oslLoadImageFilePNG("system/home/icons/transbackground.png", OSL_IN_RAM, OSL_PF_8888);
	
	while (!osl_quit)
	{	
		oslStartDrawing();

		controls();	

		oslDrawImage(transbackground);
		oslDrawImageXY(debug, 40, 48);
		
		if (benchmarkDebugActivate == 1 && benchmarkDebugActivate != 0)
			oslSysBenchmarkDisplay();
		
		if (osl_keys->pressed.cross)
		{
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

void oslPrintTextDiffColors(int x, int y, char * text, OSL_COLOR color)
 {
	oslSetTextColor(color);
	oslIntraFontSetStyle(pgfFont, 0.5, color, RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	oslSetBkColor(RGBA(0,0,0,0));
	oslDrawString(x,y,text);
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
		
	//Touch tones
	if (osl_keys->pressed.cross) oslPlaySound(tone, 1);         
	// It loads the sound file defined in the tones variable  when the cross button is pressed, in channel 1.
}

void battery(int batx, int baty, int n) // Draws the battery icon depending on its percentage. 
{
	int batteryLife;
	
	batteryLife = scePowerGetBatteryLifePercent(); //Gets battery percentage
	
	if (batteryLife <= 99 && n == 0)
	{
		batx = 343;
		baty = 2;
		oslDrawStringf(360, 4,"%d%%",batteryLife);
	}
	else  if (batteryLife > 99 && n == 0)
	{
		oslDrawStringf(352, 4,"%d%%",batteryLife);
	}
	
	if (batteryLife <= 99 && n == 1)
	{
		batx = 375;
		baty = 2;
		oslDrawStringf(392, 4,"%d%%",batteryLife);
	}
	else  if (batteryLife > 99 && n == 1)
	{
		oslDrawStringf(384, 4,"%d%%",batteryLife);
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
		oslDrawImageXY(batt0,batx,baty);
			
	if (scePowerIsBatteryCharging() == 1) // If the battery's charging, draw the charging icon on the battery icon.
		oslDrawImageXY(battcharge,batx,baty);
	
	if (isPlaying == 1)
		oslDrawImageXY(playing,5,5);
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
	if (n == 1)
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
	
	else if (n == 2)
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
	SceCtrlData pad;

	int notif_up;
	int notif_down;
	int notif_enabled;
	
	int llimit = 20;
	int rlimit = 420;
	
	oslDrawImageXY(quickSettings,0,notif_y);
		
	getDayOfWeek(15,yPos2+5);
	getMonthOfYear(65,yPos2+5);

	oslDrawStringf(137,yLine1, "Wi-Fi");
	oslDrawStringf(340,yPos2, "%d%%",scePowerGetBatteryLifePercent());
	oslDrawStringf(292,yLine1, "Balanced");
		
	oslDrawStringf(202,yLine2, "Lockscreen");
	
	oslDrawImageXY(control,controlX,yPos2+66);	
		
	digitaltime(10,yPos1,48);
	
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
	
	sceCtrlReadBufferPositive(&pad, 1);
	
	notif_enabled = 0;
	
	if (osl_pad.held.cross && cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 0 && cursor->y <= 1) 
	{
		notif_down = 1;
	}

	else if (osl_pad.held.cross && cursor->x >= 0 && cursor->x <= 480 && cursor->y >= 250 && notif_y == 0)
	{
		notif_up = 1;
	}
	
		if (osl_keys->held.up)
		{
			controlX += 4;
			setBrightness(getBrightness() + 1);
		}
		
		if (osl_keys->held.down)
		{
			controlX -= 4;
			setBrightness(getBrightness() - 1);
		}
		
		if (controlX <= llimit)
		{
			controlX = llimit;
		}
		
		else if (controlX >= rlimit)
		{
			controlX = rlimit;
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

			if (yLine2 == 200)
			{
				notif_enabled = 1;
			}
	}

	if (notif_enabled ==1)
			{
			
				if (cursor->x >= 80 && cursor->x <= 158 && cursor->y >= 41 && cursor->y <= 135 && osl_keys->pressed.cross)
				{	
					notif_y = notif_y-272;
					yPos1 = yPos1-272;
					yPos2 = yPos2-272;
					yLine1 = yLine1-272;
					yLine2 = yLine2-272;
					settingsMenu();
				}
		
				if (cursor->x >= 162 && cursor->x <= 239 && cursor->y >= 41 && cursor->y <= 135 && osl_keys->pressed.cross)
				{	
					notif_y = notif_y-272;
					yPos1 = yPos1-272;
					yPos2 = yPos2-272;
					yLine1 = yLine1-272;
					yLine2 = yLine2-272;
					wifiMenu();
				}
				
				if (cursor->x >= 445 && cursor->x <= 473 && cursor->y >= 9 && cursor->y <= 33 && osl_keys->pressed.cross && notif_down == 1)
				{	
					notif_y = notif_y-272;
					yPos1 = yPos1-272;
					yPos2 = yPos2-272;
					yLine1 = yLine1-272;
					yLine2 = yLine2-272;
					deviceStandby();
				}
 		
				if (cursor->x >= 1 && cursor->x <= 78 && cursor->y >= 138 && cursor->y <= 232 && osl_keys->pressed.cross)
				{	
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

void internet() //Draws the browser
{
	int skip = 0;
    int browser = 0;
	
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
            int res = oslBrowserInit("http://www.google.com", "/PSP/GAME/CyanogenPSP/downloads", 5*1024*1024, //Downloads will be saved into this directory
                                         PSP_UTILITY_HTMLVIEWER_DISPLAYMODE_SMART_FIT,
                                         PSP_UTILITY_HTMLVIEWER_DISABLE_STARTUP_LIMITS,
                                         PSP_UTILITY_HTMLVIEWER_INTERFACEMODE_FULL,
                                         PSP_UTILITY_HTMLVIEWER_CONNECTMODE_MANUAL_ALL);
			memset(message, 0, sizeof(message));

        }
    }
	oslNetTerm();
}

void openGmail() //Opens GMAIL in the browser.
{
	int skip = 0;
    int browser = 0;
	
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
            int res = oslBrowserInit("https://mail.google.com/mail/x/", "/PSP/GAME/CyanogenPSP/downloads", 5*1024*1024,
                                         PSP_UTILITY_HTMLVIEWER_DISPLAYMODE_SMART_FIT,
                                         PSP_UTILITY_HTMLVIEWER_DISABLE_STARTUP_LIMITS,
                                         PSP_UTILITY_HTMLVIEWER_INTERFACEMODE_FULL,
                                         PSP_UTILITY_HTMLVIEWER_CONNECTMODE_MANUAL_ALL);
			memset(message, 0, sizeof(message));

        }
    }
	oslNetTerm();
}

//First Boot Message
void firstBootMessage()
{	
	int firstBoot;

	FILE * firstBootTxt = fopen("system/firstBoot.txt", "r");
	
	if (fileExists("system/firstBoot.txt"))
	{
		fscanf(firstBootTxt,"%d",&firstBoot);
		fclose(firstBootTxt);
	}
	else
	{
		firstBootTxt = fopen("system/firstBoot.txt", "w");
		fprintf(firstBootTxt, "1", firstBoot);
		fclose(firstBootTxt);
	}

	if (firstBoot!= 0)
	{
		fclose(firstBootTxt);
	
		while (!osl_quit)
		{		
			oslStartDrawing();

			controls();

			oslDrawImage(background);		
			oslDrawImageXY(music, 105, 190);
			oslDrawImageXY(browser, 276, 190);
			oslDrawImageXY(gmail, 331, 190);
			oslDrawImageXY(messengericon, 160, 190);
			oslDrawImageXY(pointer, 230, 180);
		
			digitaltime(420,4,458); 
		
			oslSetTransparentColor(RGB(0,0,0));
			appDrawerIcon();
			oslDisableTransparentColor();
		
			battery(370,2,1);
			navbarButtons(1);
		
			oslDrawImageXY(transbackground, 0, 0);
			oslDrawImageXY(welcome, 0, 0);
			oslDrawImage(cursor);
	
			if (cursor->x >= 366 && cursor->x <= 442 && cursor->y >= 80 && cursor->y <= 116 && osl_keys->pressed.cross)
			{
				firstBootTxt = fopen("system/firstBoot.txt", "w"); 
				fprintf(firstBootTxt, "0", firstBoot);
				fclose(firstBootTxt);
				oslDeleteImage(welcome);
				oslDeleteImage(transbackground);
				unloadIcons();
				home();
			}
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
		}
	}
		
	if (firstBoot == 0)
	{
		oslDeleteImage(welcome);
		oslDeleteImage(transbackground);
		unloadIcons();
		home();
	}
}

void setfont() //Sets the main font (size and color) used in the app
{
	oslIntraFontSetStyle(DroidSans, 0.5f, RGBA(0,0,0,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	oslSetFont(DroidSans);
}

void loadIcons() // Loading the app drawer icons.
{
	ic_allapps = oslLoadImageFilePNG("system/framework/framework-res/res/drawable-hdpi/ic_allapps.png", OSL_IN_RAM, OSL_PF_8888);
	ic_allapps_pressed = oslLoadImageFile("system/framework/framework-res/res/drawable-hdpi/ic_allapps_pressed.png", OSL_IN_RAM, OSL_PF_8888);
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

/*

int brightnessControl() 
{	
	control = oslLoadImageFilePNG("system/home/menu/brightnesscontrol.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!control)
		debugDisplay();
	
	pspDebugScreenInit();
	
	SceCtrlData pad;
	
	control->y = 148;
		
	int llimit = 120;
	int rlimit = 335;
	
	while (!osl_quit)
	{
		oslStartDrawing();
		oslReadKeys();
		
		if (getBrightness() == 10)
		{
			control->x = 120;
		}
		else if (getBrightness() == 20)
		{
			control->x = 147;
		}
		else if (getBrightness() == 30)
		{
			control->x = 174;
		}
		else if (getBrightness() == 40)
		{
			control->x = 201;
		}
		else if (getBrightness() == 50)
		{
			control->x = 228;
		}
		else if (getBrightness() == 60)
		{
			control->x = 255;
		}
		else if (getBrightness() == 70)
		{
			control->x = 282;
		}
		else if (getBrightness() == 80)
		{
			control->x = 309;
		}
		else if (getBrightness() == 90)
		{
			control->x = 335;
		}
		
		oslDrawImage(control);
	
		if (osl_keys->held.up)
		{
			control->x += 3;
		}
		
		if (osl_keys->held.down)
		{
			control->x -= 3;
		}
		
		if (control->x <= llimit)
		{
			control->x = llimit;
		}
		
		else if (control->x >= rlimit)
		{
			control->x = rlimit;
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

		sceCtrlReadBufferPositive(&pad, 1);
	
		if (pad.Buttons & PSP_CTRL_CIRCLE) 
		{
			oslDeleteImage(brightness);
			oslDeleteImage(control);
			return;
		}
	
		if (pad.Buttons & PSP_CTRL_UP) 
		{
			setBrightness(getBrightness() + 1);
		}
	
		if (pad.Buttons & PSP_CTRL_DOWN) 
		{
			setBrightness(getBrightness() - 1);
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}
*/

int sysButtons() 
{		
	pspDebugScreenInit();
	
	setfont();
	
	SceCtrlData pad;
	
	SceUID modid = pspSdkLoadStartModule("modules/sysbuttons.prx", PSP_MEMORY_PARTITION_KERNEL);
	
	while (!osl_quit)
	{
		oslStartDrawing();
		oslReadKeys();
		
		if (readHomeButton)
		{
			powermenu();
		}

		if (readVolUpButton)
		{
			oslDrawStringf(240,136,"Volume up initiated");
		}
	
		if (readVolDownButton) 
		{
			oslDrawStringf(240,146,"Volume down initiated");
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
}

int bootMenu()
{
	setfont();

	while (!osl_quit)
	{
		oslStartDrawing();
		
		oslCls();
	
		oslPrintf("\n CyanogenPSP BootMenu\n");
		
		sceKernelDelayThread(3000000);
		
		oslPrintf("\n Checking Modules...\n");
		
		sceKernelDelayThread(3000000);
		
		oslPrintf("\n\n loading modules ...\n");   
		if(pspSdkLoadStartModule("brightness.prx", PSP_MEMORY_PARTITION_KERNEL) < 0)   
		{   
			oslPrintf(" Error loading module brightness.prx\n");   
			sceKernelDelayThread(5000000);   
			return -1;   
		}   
		
		oslPrintf("\n - brightness.prx loaded\n");
		
		sceKernelDelayThread(3000000);
		
		oslPrintf("\n\n Done!");
	
		//Ends Printing and Drawing
		oslEndDrawing(); 

		//End's Frame
        oslEndFrame(); 
		
		//Synchronizes the screen 
		oslSyncFrame();	
	}
}

int main()
{
	SetupCallbacks(); //Setup callbacks
	initOSLib(); //Initializes OsLib
	oslIntraFontInit(INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8); //Initializes OSL fonts
	
	//Loads our touch tones
	tone = oslLoadSoundFile("system/media/audio/ui/KeypressStandard.wav", OSL_FMT_NONE);

	FILE * backgroundPathTXT;

	if (!(fileExists("system/framework/framework-res/res/background.txt")))
	{
		backgroundPathTXT = fopen("system/framework/framework-res/res/background.txt", "w");
		fprintf(backgroundPathTXT, "system/framework/framework-res/res/background.png");
		fclose(backgroundPathTXT);
	}
	
	backgroundPathTXT = fopen("system/framework/framework-res/res/background.txt", "r");
	fscanf(backgroundPathTXT,"%s",backgroundPath);

	//Loads our images into memory
	loadIcons();
	background = oslLoadImageFile(backgroundPath, OSL_IN_RAM, OSL_PF_8888);
	cursor = oslLoadImageFilePNG("system/cursor/cursor.png", OSL_IN_VRAM, OSL_PF_8888);
	navbar = oslLoadImageFile("system/home/icons/nav.png", OSL_IN_VRAM, OSL_PF_8888);
	navbar2 = oslLoadImageFile("system/home/icons/nav2.png", OSL_IN_RAM, OSL_PF_8888);
	wificon = oslLoadImageFile("system/home/icons/wificon.png", OSL_IN_VRAM, OSL_PF_8888);
	music = oslLoadImageFilePNG("system/home/icons/apollo.png", OSL_IN_RAM, OSL_PF_8888);
	gmail = oslLoadImageFilePNG("system/home/icons/gmail.png", OSL_IN_RAM, OSL_PF_8888);
	messengericon = oslLoadImageFilePNG("system/home/icons/message.png", OSL_IN_RAM, OSL_PF_8888);
	browser = oslLoadImageFile("system/home/icons/browser.png", OSL_IN_RAM, OSL_PF_8888);
	quickSettings = oslLoadImageFile("system/home/menu/quickSettings.png", OSL_IN_VRAM, OSL_PF_8888);
	notif = oslLoadImageFile("system/home/menu/notif.png", OSL_IN_RAM, OSL_PF_8888);
	batt100 = oslLoadImageFile("system/home/icons/100.png", OSL_IN_VRAM, OSL_PF_8888);
	batt80 = oslLoadImageFile("system/home/icons/80.png", OSL_IN_VRAM, OSL_PF_8888);
	batt60 = oslLoadImageFile("system/home/icons/60.png", OSL_IN_VRAM, OSL_PF_8888);
	batt40 = oslLoadImageFile("system/home/icons/40.png", OSL_IN_VRAM, OSL_PF_8888);
	batt20 = oslLoadImageFile("system/home/icons/20.png", OSL_IN_VRAM, OSL_PF_8888);
	batt10 = oslLoadImageFile("system/home/icons/10.png", OSL_IN_VRAM, OSL_PF_8888);
	batt0 = oslLoadImageFile("system/home/icons/0.png", OSL_IN_VRAM, OSL_PF_8888);
	battcharge = oslLoadImageFile("system/home/icons/charge.png", OSL_IN_VRAM, OSL_PF_8888);
	pointer = oslLoadImageFilePNG("system/home/icons/pointer.png", OSL_IN_RAM, OSL_PF_8888);
	pointer1 = oslLoadImageFilePNG("system/home/icons/pointer1.png", OSL_IN_RAM, OSL_PF_8888);
	backicon = oslLoadImageFilePNG("system/home/icons/backicon.png", OSL_IN_RAM, OSL_PF_8888);
	homeicon = oslLoadImageFilePNG("system/home/icons/homeicon.png", OSL_IN_RAM, OSL_PF_8888);
	multicon = oslLoadImageFilePNG("system/home/icons/multicon.png", OSL_IN_RAM, OSL_PF_8888);
	backicon2 = oslLoadImageFilePNG("system/home/icons/backicon2.png", OSL_IN_RAM, OSL_PF_8888);
	homeicon2 = oslLoadImageFilePNG("system/home/icons/homeicon2.png", OSL_IN_RAM, OSL_PF_8888);
	multicon2 = oslLoadImageFilePNG("system/home/icons/multicon2.png", OSL_IN_RAM, OSL_PF_8888);
	welcome = oslLoadImageFilePNG("system/home/icons/welcome.png", OSL_IN_RAM, OSL_PF_8888);
	transbackground = oslLoadImageFilePNG("system/home/icons/transbackground.png", OSL_IN_RAM, OSL_PF_8888);
	playing = oslLoadImageFilePNG("system/home/icons/playing.png", OSL_IN_VRAM, OSL_PF_8888);
	control = oslLoadImageFilePNG("system/home/menu/brightnesscontrol.png", OSL_IN_RAM, OSL_PF_8888);
	
	DroidSans = oslLoadIntraFontFile("system/fonts/DroidSans.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	
	SceUID modid;
	
	modid = pspSdkLoadStartModule("modules/brightness.prx", PSP_MEMORY_PARTITION_KERNEL);

	//Debugger - Displays an error message if the following resources are missing.
	if (!background || !cursor || !ic_allapps || !ic_allapps_pressed || !navbar || !wificon || !music || !gmail || !messengericon || !browser || !notif || !batt100 || !batt80 || !batt60 || !batt40 || !batt20 || !batt10 || !batt0 || !battcharge || !pointer || !pointer1 || !backicon || !multicon || !homeicon)
		debugDisplay();
	
	loadConfig();
	
	//Sets the cursor's original position on the screen
	cursor->x = 240;
	cursor->y = 136;
	
	setfont();
	
	makeDownloadDir(); //Created Download directory if there isn't any - PSP/Game/CyanogenMod/Downloads
	deleteUpdateFile(); //Delete update.zip
	
	fclose(backgroundPathTXT);
	
	//Main loop to run the program
	while (!osl_quit)
	{		
		//Draws images onto the screen
		oslStartDrawing();
		
		//Initiate the PSP's controls
		controls();
			
		//Print the images onto the screen
		oslDrawImage(background);		
		oslDrawImageXY(music, 105, 190);
		oslDrawImageXY(browser, 276, 190);
		oslDrawImageXY(gmail, 331, 190);
		oslDrawImageXY(messengericon, 160, 190);
		oslDrawImageXY(pointer, 232, 180);
		
		digitaltime(420,4,458); //Draws digital time (based on your local psp time) on the top right corner. 

		//Sets the transparency color (black)
		oslSetTransparentColor(RGB(0,0,0));
		
		appDrawerIcon();
		
		//Disables the transparent color
		oslDisableTransparentColor();
		
		battery(370,2,1);
		navbarButtons(1);
		androidQuickSettings();
		firstBootMessage();
		oslDrawImage(cursor);

		if (osl_keys->pressed.square) //Opens the power menu
		{
			 powermenu();
		}
				
		//Launching the browser
		if (cursor->x >= 276 && cursor->x <= 321 && cursor->y >= 190 && cursor->y <= 240 && osl_keys->pressed.cross) //Launches the internet
		{
			unloadIcons();
			internet();
		}
		
		if (cursor->x >= 330 && cursor->x <= 374 && cursor->y >= 190 && cursor->y <= 240 && osl_keys->pressed.cross) //Opens Gmail
		{
			unloadIcons();
			openGmail();
		}
		
		if (cursor->x >= 100 && cursor->x <= 154 && cursor->y >= 190 && cursor->y <= 240 && osl_keys->pressed.cross) //Opens music MP3 player
		{
			unloadIcons();
			mp3player();
		}
		
		/*Messenger
		if (cursor->x >= 155 && cursor->x <= 210 && cursor->y >= 190 && cursor->y <= 240 && osl_keys->pressed.cross) //Opens messenger
		{
			unloadIcons();
			messenger();
		}
		*/
			
		if (cursor->x >= 215 && cursor->x <= 243 && cursor->y >= 195 && cursor->y <= 230 && osl_keys->pressed.cross) //Opens app drawer
		{
			unloadIcons();
			appdrawer();
		}
		
		if (osl_keys->pressed.L) //Locks the screen
		{
			lockscreen();
        }
		
		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross) // Opens multi-task menu
		{
			unloadIcons();
			multitask();
		}
		
		if (osl_pad.held.R && osl_keys->pressed.triangle) //Takes screenshot
		{
			screenshot();
		}
	
		//Ends Printing and Drawing
		oslEndDrawing(); 

		//End's Frame
        oslEndFrame(); 
		
		//Synchronizes the screen 
		oslSyncFrame();	
	}
	
	//Terminates/Ends the program
	oslQuit();
	return 0;
}

