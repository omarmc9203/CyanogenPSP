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
#include "messenger.h"
#include "power_menu.h"
#include "recoverymenu.h"
#include "settingsmenu.h"

#include "screenshot.h"
#include "include/ram.h"
#include "include/utils.h"

PSP_MODULE_INFO("CyanogenPSP",  1, 5, 2);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU); 
PSP_HEAP_SIZE_KB(20*1024); //This line will be altered for slims. Right now us (20480) when building for PSP 1000s, and (53248) for testing on PPSSPP. Using heap_max breaks the browser so don't use it.

int initOSLib() //Initialize OsLib
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

int imposeSetBrightness(int value);
int imposeGetBrightness();
 
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

void set_volume(int vol) {
	if(vol > 30)
		vol = 30;
	if(vol < 0)
		vol = 0;
		
	imposeSetVolume(vol);
}

void increase_volume(int n) {
	int v = imposeGetVolume();
	
	set_volume(v+n);
}

void decrease_volume(int n) {
	int v = imposeGetVolume();
	
	set_volume(v-n);
}

//First Boot Message
void firstBootMessage()
{	
	int firstBoot;

	FILE * firstBootTxt;
	
	if (fileExists("system/firstBoot.txt"))
	{
		firstBootTxt = fopen("system/firstBoot.txt", "r");
		fscanf(firstBootTxt,"%d",&firstBoot);
		fclose(firstBootTxt);
	}
	else
	{
		firstBootTxt = fopen("system/firstBoot.txt", "w");
		fprintf(firstBootTxt, "1");
		fclose(firstBootTxt);
	}
	
	firstBootTxt = fopen("system/firstBoot.txt", "r");
	fscanf(firstBootTxt,"%d",&firstBoot);
	fclose(firstBootTxt);

	if (firstBoot!= 0)
	{
		fclose(firstBootTxt);
	
		while (!osl_quit)
		{		
			oslStartDrawing();

			controls();

			oslDrawImage(background);		
			oslDrawImageXY(ic_launcher_apollo, 105, 190);
			oslDrawImageXY(ic_launcher_browser, 276, 190);
			oslDrawImageXY(ic_launcher_settings, 331, 190);
			oslDrawImageXY(ic_launcher_messenger, 160, 190);
			oslDrawImageXY(pointer, 230, 180);
		
			digitaltime(420,4,0);
		
			oslSetTransparentColor(RGB(0,0,0));
			appDrawerIcon();
			oslDisableTransparentColor();
		
			battery(370,2,1);
			navbarButtons(0);
		
			oslDrawImageXY(transbackground, 0, 0);
			oslDrawImageXY(welcome, 0, 0);
			oslDrawImage(cursor);
	
			if (cursor->x >= 366 && cursor->x <= 442 && cursor->y >= 80 && cursor->y <= 116 && osl_keys->pressed.cross)
			{ 
				firstBootTxt = fopen("system/firstBoot.txt", "w"); 
				fprintf(firstBootTxt, "0");
				fclose(firstBootTxt);
				oslPlaySound(KeypressStandard, 1); 
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
		lockscreen();
		home();
	}
}

void createDirs()
{
	SceUID dir = sceIoDopen("ms0:/PICTUREO");
	
	if (dirExists("ms0:/PICTURE"))
	{
		sceIoDclose(dir);
	}
	else 
	{
		sceIoMkdir("ms0:/PICTURE",0777);
	}

	SceUID dir1 = sceIoDopen("ms0:/PSP/PHOTO");
	
	if (dirExists("ms0:/PSP/PHOTO"))
	{
		sceIoDclose(dir1);
	}
	else 
	{
		sceIoMkdir("ms0:/PSP/PHOTO",0777);
	}
	
	SceUID dir2 = sceIoDopen("ms0:/PSP/GAME/CyanogenPSP/screenshots");
	
	if (dirExists("ms0:/PSP/GAME/CyanogenPSP/screenshots"))
	{
		sceIoDclose(dir2);
	}
	else 
	{
		sceIoMkdir("ms0:/PSP/GAME/CyanogenPSP/screenshots",0777);
	}
	
	SceUID dir3 = sceIoDopen("ms0:/MUSIC");
	
	if (dirExists("ms0:/MUSIC"))
	{
		sceIoDclose(dir3);
	}
	else 
	{
		sceIoMkdir("ms0:/MUSIC",0777);
	}
	
	SceUID dir4 = sceIoDopen("ms0:/PSP/MUSIC");
	
	if (dirExists("ms0:/PSP/MUSIC"))
	{
		sceIoDclose(dir4);
	}
	else 
	{
		sceIoMkdir("ms0:/PSP/MUSIC",0777);
	}
	
	SceUID dir5 = sceIoDopen("ms0:/PSP/GAME/CyanogenPSP/downloads");
	
	if (dirExists("ms0:/PSP/GAME/CyanogenPSP/downloads"))
	{
		sceIoDclose(dir5);
	}
	else 
	{
		sceIoMkdir("ms0:/PSP/GAME/CyanogenPSP/downloads",0777);
	}
	
	SceUID dir6 = sceIoDopen("ms0:/ISO");
	
	if (dirExists("ms0:/ISO"))
	{
		sceIoDclose(dir6);
	}
	else 
	{
		sceIoMkdir("ms0:/ISO",0777);
	}
}

int main()
{
	SetupCallbacks(); //Setup callbacks
	initOSLib(); //Initializes OsLib
	oslIntraFontInit(INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8); //Initializes OSL fonts
	
	//Loads our tones
	camera_click = oslLoadSoundFile("system/media/audio/ui/camera_click.wav", OSL_FMT_NONE);
	KeypressStandard = oslLoadSoundFile("system/media/audio/ui/KeypressStandard.wav", OSL_FMT_NONE);
	Lock = oslLoadSoundFile("system/media/audio/ui/Lock.wav", OSL_FMT_NONE);
	LowBattery = oslLoadSoundFile("system/media/audio/ui/LowBattery.wav", OSL_FMT_NONE);
	Unlock = oslLoadSoundFile("system/media/audio/ui/Unlock.wav", OSL_FMT_NONE);
	WirelessChargingStarted = oslLoadSoundFile("system/media/audio/ui/WirelessChargingStarted.wav", OSL_FMT_NONE);

	FILE * backgroundPathTXT;
	
	if (!(fileExists("system/framework/framework-res/res/background.txt")))
	{
		backgroundPathTXT = fopen("system/framework/framework-res/res/background.txt", "w");
		fprintf(backgroundPathTXT, "system/framework/framework-res/res/background.png");
		fclose(backgroundPathTXT);
	}
	
	backgroundPathTXT = fopen("system/framework/framework-res/res/background.txt", "r");
	fscanf(backgroundPathTXT,"%s",backgroundPath);
	fclose(backgroundPathTXT);
	
	FILE * fontPathTXT;
	
	if (!(fileExists("system/fonts/fonts.txt")))
	{
		fontPathTXT = fopen("system/fonts/fonts.txt", "w");
		fprintf(fontPathTXT, "system/fonts/Roboto.pgf");
		fclose(fontPathTXT);
	}
	
	fontPathTXT = fopen("system/fonts/fonts.txt", "r");
	fscanf(fontPathTXT,"%s",fontPath);
	fclose(fontPathTXT);

	createDirs();
	
	//Loads our images into memory
	loadIcons();
	background = oslLoadImageFile(backgroundPath, OSL_IN_RAM, OSL_PF_8888);
	cursor = oslLoadImageFilePNG("system/cursor/cursor.png", OSL_IN_VRAM, OSL_PF_8888);
	navbar = oslLoadImageFile("system/home/icons/nav.png", OSL_IN_VRAM, OSL_PF_8888);
	navbar2 = oslLoadImageFile("system/home/icons/nav2.png", OSL_IN_RAM, OSL_PF_8888);
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
	control = oslLoadImageFilePNG("system/home/menu/brightnesscontrol.png", OSL_IN_VRAM, OSL_PF_8888);
	ic_launcher_apollo = oslLoadImageFilePNG("system/app/apollo/ic_launcher_apollo.png", OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_messenger = oslLoadImageFilePNG("system/app/messenger/ic_launcher_messenger.png", OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_browser = oslLoadImageFile("system/app/browser/ic_launcher_browser.png", OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_filemanager = oslLoadImageFilePNG("system/app/filemanager/ic_launcher_filemanager.png", OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_settings = oslLoadImageFilePNG("system/settings/ic_launcher_settings.png", OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_gallery = oslLoadImageFilePNG("system/app/gallery/ic_launcher_gallery.png", OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_umd = oslLoadImageFilePNG("system/app/umd/ic_launcher_umd.png", OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_calculator = oslLoadImageFilePNG("system/app/calculator/ic_launcher_calculator.png", OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_game = oslLoadImageFilePNG("system/app/game/ic_launcher_game.png", OSL_IN_RAM, OSL_PF_8888);
	layerA = oslLoadImageFilePNG("system/home/icons/layerA.png", OSL_IN_RAM, OSL_PF_8888);
	layerB = oslLoadImageFilePNG("system/home/icons/layerB.png", OSL_IN_RAM, OSL_PF_8888);
	volumeBar = oslLoadImageFilePNG("system/volumeBar.png", OSL_IN_RAM, OSL_PF_8888);
	volumeControl = oslLoadImageFile("system/volumeControl.png", OSL_IN_RAM, OSL_PF_8888);
	
	Roboto = oslLoadIntraFontFile(fontPath, INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslSetFont(Roboto);
	
	SceUID modid; //, modid2;
	
	modid = pspSdkLoadStartModule("modules/display.prx", PSP_MEMORY_PARTITION_KERNEL);
	//modid2 = pspSdkLoadStartModule("modules/sound.prx", PSP_MEMORY_PARTITION_KERNEL);
	
	//Debugger - Displays an error message if the following resources are missing.
	if (!background || !cursor || !ic_allapps || !ic_allapps_pressed || !navbar || !ic_launcher_apollo || !ic_launcher_settings || !ic_launcher_messenger || !ic_launcher_browser || !notif || !batt100 || !batt80 || !batt60 || !batt40 || !batt20 || !batt10 || !batt0 || !battcharge || !pointer || !pointer1 || !backicon || !multicon || !homeicon)
		debugDisplay();
	
	loadConfig();
	
	//Sets the cursor's original position on the screen
	cursor->x = 240;
	cursor->y = 136;
	
	makeDownloadDir(); //Created Download directory if there isn't any - PSP/Game/CyanogenMod/Downloads
	deleteUpdateFile(); //Delete update.zip
	
	setCpuBoot();
	
	//Main loop to run the program
	while (!osl_quit)
	{		
		firstBootMessage();
	
		//Draws images onto the screen
		oslStartDrawing();
		
		//Initiate the PSP's controls
		controls();
			
		//Print the images onto the screen
		oslDrawImage(background);		
		oslDrawImageXY(ic_launcher_apollo, 105, 190);
		oslDrawImageXY(ic_launcher_browser, 276, 190);
		oslDrawImageXY(ic_launcher_settings, 331, 190);
		oslDrawImageXY(ic_launcher_messenger, 160, 190);
		oslDrawImageXY(pointer, 232, 180); 

		//Sets the transparency color (black)
		oslSetTransparentColor(RGB(0,0,0));
		
		appDrawerIcon();
		
		//Disables the transparent color
		oslDisableTransparentColor();

		navbarButtons(0);
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
		digitaltime(420,4,0);//Draws digital time (based on your local psp time) on the top right corner.
		battery(370,2,1);
		androidQuickSettings();
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
			settingsMenu();
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

