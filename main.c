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
#include "include/utils.h"

PSP_MODULE_INFO("CyanogenPSP",  1, 4, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU); 
PSP_HEAP_SIZE_KB(24000);

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

/*
int sysButtons() 
{		
	pspDebugScreenInit();
	
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
*/

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
	
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslSetFont(Roboto);
	
	SceUID modid;
	
	modid = pspSdkLoadStartModule("modules/brightness.prx", PSP_MEMORY_PARTITION_KERNEL);

	//Debugger - Displays an error message if the following resources are missing.
	if (!background || !cursor || !ic_allapps || !ic_allapps_pressed || !navbar || !wificon || !music || !gmail || !messengericon || !browser || !notif || !batt100 || !batt80 || !batt60 || !batt40 || !batt20 || !batt10 || !batt0 || !battcharge || !pointer || !pointer1 || !backicon || !multicon || !homeicon)
		debugDisplay();
	
	loadConfig();
	
	//Sets the cursor's original position on the screen
	cursor->x = 240;
	cursor->y = 136;
	
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

		//Sets the transparency color (black)
		oslSetTransparentColor(RGB(0,0,0));
		
		appDrawerIcon();
		
		//Disables the transparent color
		oslDisableTransparentColor();
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,0);
		
		navbarButtons(1);
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
		digitaltime(420,4,458); //Draws digital time (based on your local psp time) on the top right corner.
		battery(370,2,1);
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

