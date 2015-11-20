//Parts of the Shell
#include "musicPlayer.h"
#include "appDrawer.h"
#include "language.h"
#include "calculator.h"
#include "clock.h"
#include "fileManager.h"
#include "gameLauncher.h"
#include "homeMenu.h"
#include "lockScreen.h"
#include "main.h"
#include "recentsMenu.h"
#include "messenger.h"
#include "powerMenu.h"
#include "recoveryMenu.h"
#include "settingsMenu.h"

#include "screenshot.h"
#include "include/ram.h"
#include "include/utils.h"

PSP_MODULE_INFO("CyanogenPSP",  1, 5, 2);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU); 
PSP_HEAP_SIZE_KB(20*1024); //This line will be altered for slims. Right now us (20480) when building for PSP 1000s, and (53248) for testing on PPSSPP. Using heap_max breaks the browser so don't use it.

int initOSLib() //Initialize OsLib
{
    oslInit(1);
    oslInitGfx(OSL_PF_8888, 1);
	oslSetBilinearFilter(1);
    oslInitAudio();
    oslSetKeyAutorepeatInit(40);
    oslSetKeyAutorepeatInterval(10);
    return 0;
}

void langDisplay()
{	
	oslDrawImage(langSelection);
	
	oslIntraFontSetStyle(Roboto, 0.7f, WHITE, 0, INTRAFONT_ALIGN_LEFT);
	oslDrawStringf(20, 85, "CyanogenPSP");
	oslIntraFontSetStyle(Roboto, 0.65f, WHITE, 0, INTRAFONT_ALIGN_LEFT);
	oslDrawStringf(20, 110, "%s", lang_welcome[language][0]);
	
	oslIntraFontSetStyle(Roboto, 0.5f, WHITE, 0, INTRAFONT_ALIGN_LEFT);
	digitaltime(420,4,0,hrTime);
	battery(370,2,1);		
	
	oslDrawFillRect(60, 179, 420, 180, LITEGRAY);
	oslDrawFillRect(60, 217, 420, 218, LITEGRAY);

	for(i = curScroll; i < MAX_LANG_DISPLAY + curScroll; i++) 
	{
		if ((folderIcons[i].active == 0) && (current >= i-1)) 
		{
			current = i-1;
			break;
		}

		if (current <= curScroll-1) 
		{
			current = curScroll-1;
			break;
		}

		if (folderIcons[i].active == 1) 
		{
			oslIntraFontSetStyle(Roboto, 0.55f, BLACK, 0, INTRAFONT_ALIGN_CENTER);
			oslDrawStringf(LANG_DISPLAY_X, (i - curScroll)*36 + LANG_DISPLAY_Y, "%.56s", folderIcons[i].name);	
		}
	}
}

void langScrollUp()
{
	current--;
	if ((current-1 <= curScroll-1) && (curScroll > 1)) {
		curScroll--; 
	}
}

void langScrollDown()
{
	if (folderIcons[current+1].active) current++;
	if (current >= (MAX_LANG_DISPLAY+curScroll-1)) {
		curScroll++; 
	}
}

void langControls()
{
	oslReadKeys();

	if (pad.Buttons != oldpad.Buttons) 
	{
		if (osl_keys->pressed.down)
		{
			langScrollDown();
			if (strcmp(folderIcons[current].filePath, "doesn't matter") != 0) 
				changeLanguage();
			timer = 0;
		}
		else if (osl_keys->pressed.up)
		{
			langScrollUp();
			if (strcmp(folderIcons[current].filePath, "doesn't matter") != 0)
				changeLanguage();
			timer = 0;
		}
	}
	
	if ((osl_keys->pressed.cross) && (strcmp(folderIcons[current].filePath, "doesn't matter") != 0))
	{
		oslPlaySound(KeypressStandard, 1); 
		language = setFileDefaultsInt("system/settings/language.bin", 0, language);
		oslDeleteImage(langSelection);
		firstBootInitiation();
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

char * langBrowse(const char * path)
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
		langDisplay();

		langControls(); //0 is to used for selecting a font
			
		if (strlen(returnMe) > 4) 
			break;
			
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();
	}
	return returnMe;
}

void displayLangSelection(char * browseDirectory)
{	
	langSelection = oslLoadImageFilePNG("system/settings/langSelection.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	browseDirectory = langBrowse("system/settings/language"); //For language

	if (!langSelection)
		debugDisplay();
	
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

int firstBoot;

//First Boot Message
void firstBootInitiation()
{	
	firstBoot = setFileDefaultsInt("system/settings/boot.bin", 1, firstBoot);

	iconPackLoad();

	//Loads our images into memory
	loadIcons();
	background = oslLoadImageFile(backgroundPath, OSL_IN_RAM, OSL_PF_8888);
	cursor = oslLoadImageFilePNG(cursorPath, OSL_IN_VRAM, OSL_PF_8888);
	navbar2 = oslLoadImageFile("system/home/icons/nav2.png", OSL_IN_RAM, OSL_PF_8888);
	navbar = oslLoadImageFile("system/home/icons/nav.png", OSL_IN_VRAM, OSL_PF_8888);
	notif = oslLoadImageFile("system/home/menu/notif.png", OSL_IN_RAM, OSL_PF_8888);
	backicon = oslLoadImageFilePNG(backiconPath, OSL_IN_RAM, OSL_PF_8888);
	homeicon = oslLoadImageFilePNG(homeiconPath, OSL_IN_RAM, OSL_PF_8888);
	multicon = oslLoadImageFilePNG(multiconPath, OSL_IN_RAM, OSL_PF_8888);
	backicon2 = oslLoadImageFilePNG(backicon2Path, OSL_IN_RAM, OSL_PF_8888);
	homeicon2 = oslLoadImageFilePNG(homeicon2Path, OSL_IN_RAM, OSL_PF_8888);
	multicon2 = oslLoadImageFilePNG(multicon2Path, OSL_IN_RAM, OSL_PF_8888);
	welcome = oslLoadImageFilePNG("system/home/icons/welcome.png", OSL_IN_RAM, OSL_PF_8888);
	transbackground = oslLoadImageFilePNG("system/home/icons/transbackground.png", OSL_IN_RAM, OSL_PF_8888);
	control = oslLoadImageFilePNG("system/home/menu/brightnesscontrol.png", OSL_IN_VRAM, OSL_PF_8888);
	ic_launcher_apollo = oslLoadImageFilePNG(apolloPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_browser = oslLoadImageFile(browserPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_calculator = oslLoadImageFilePNG(calcPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_filemanager = oslLoadImageFilePNG(fmPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_gallery = oslLoadImageFilePNG(galleryPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_game = oslLoadImageFilePNG(gamePath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_messenger = oslLoadImageFilePNG(messagesPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_settings = oslLoadImageFilePNG(settingsPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_umd = oslLoadImageFilePNG(umdPath, OSL_IN_RAM, OSL_PF_8888);
	usbdebug = oslLoadImageFilePNG("system/home/icons/usbdebug.png", OSL_IN_RAM, OSL_PF_8888);
	music = oslLoadImageFilePNG("system/home/icons/music.png", OSL_IN_RAM, OSL_PF_8888);
	
	
	//Debugger - Displays an error message if the following resources are missing.
	if (!background || !cursor || !ic_allapps || !ic_allapps_pressed || !navbar || !ic_launcher_apollo || !ic_launcher_settings || !ic_launcher_messenger || !ic_launcher_browser || !notif || !batt100 || !batt80 || !batt60 || !batt40 || !batt20 || !batt10 || !batt0 || !battcharge || !backicon || !multicon || !homeicon || !usbdebug  || !music)
		debugDisplay();
		
	//Sets the cursor's original position on the screen
	cursor->x = 240;
	cursor->y = 136;	
	
	if (firstBoot!= 0)
	{
		while (!osl_quit)
		{		
			oslStartDrawing();

			controls();

			oslDrawImage(background);		
			oslDrawImageXY(ic_launcher_apollo, 105, 190);
			oslDrawImageXY(ic_launcher_browser, 276, 190);
			oslDrawImageXY(ic_launcher_settings, 331, 190);
			oslDrawImageXY(ic_launcher_messenger, 160, 190);
			oslSetTransparentColor(RGB(0,0,0));
			appDrawerIcon();
			oslDisableTransparentColor();
		
			oslIntraFontSetStyle(Roboto, 0.5f, WHITE, 0, INTRAFONT_ALIGN_LEFT);
			
			digitaltime(420,4,0,hrTime);
			volumeController();
			battery(370,2,1);
			navbarButtons(0);
			
			oslDrawImageXY(transbackground, 0, 0);
			oslDrawImageXY(welcome, 0, 0);
			
			oslIntraFontSetStyle(Roboto, 0.8f,BLACK,0,INTRAFONT_ALIGN_LEFT);
			oslDrawStringf(20,30, "%s", lang_welcome[language][0]);
			
			oslIntraFontSetStyle(Roboto, 0.6f,DARKGRAY,0,INTRAFONT_ALIGN_LEFT);
			oslDrawStringf(20,60, "%s", lang_welcome[language][1]);
			
			oslDrawStringf(20,80, "%s", lang_welcome[language][2]);
			
			oslIntraFontSetStyle(Roboto, 0.6f,WHITE,0,INTRAFONT_ALIGN_LEFT);
			oslDrawStringf(385,110, "%s", lang_welcome[language][3]);
			
			oslDrawImage(cursor);
	
			if (cursor->x >= 388 && cursor->x <= 464 && cursor->y >= 98 && cursor->y <= 132 && osl_keys->pressed.cross)
			{
				FILE * firstBootTxt = fopen("system/settings/boot.bin", "w"); 
				fprintf(firstBootTxt, "0");
				fclose(firstBootTxt);
				oslPlaySound(KeypressStandard, 1); 
				oslDeleteImage(welcome);
				oslDeleteImage(transbackground);
				unloadIcons();
				home();
			}
			
			captureScreenshot();
			
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

int bootAnimation()
{
	int currentFrame = 0, i = 0;
	
	bootAnimActivator = setFileDefaultsInt("system/boot/bootAnimActivator.bin", 0, bootAnimActivator);
	firstBoot = setFileDefaultsInt("system/settings/boot.bin", 1, firstBoot);
	
	if (bootAnimActivator != 1)
	{
		if (firstBoot == 1)
			displayLangSelection("system/settings/language");
		else 
			firstBootInitiation();
	}
	
	bootAnim[0] = oslLoadImageFilePNG("system/boot/part1/boot0.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[1] = oslLoadImageFilePNG("system/boot/part1/boot1.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[2] = oslLoadImageFilePNG("system/boot/part1/boot2.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[3] = oslLoadImageFilePNG("system/boot/part1/boot3.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[4] = oslLoadImageFilePNG("system/boot/part1/boot4.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[5] = oslLoadImageFilePNG("system/boot/part1/boot5.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[6] = oslLoadImageFilePNG("system/boot/part1/boot6.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[7] = oslLoadImageFilePNG("system/boot/part1/boot7.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[8] = oslLoadImageFilePNG("system/boot/part1/boot8.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[9] = oslLoadImageFilePNG("system/boot/part1/boot9.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[10] = oslLoadImageFilePNG("system/boot/part1/boot10.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[11] = oslLoadImageFilePNG("system/boot/part1/boot11.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	bootAnim[12] = oslLoadImageFilePNG("system/boot/part1/boot12.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	
	while(!osl_quit)
	{
		oslStartDrawing();
		oslDrawImageXY(bootAnim[currentFrame], 80, -25); 
		sceDisplayWaitVblankStart();
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
		sceKernelDelayThread(10000 * 2);
   
		currentFrame++;
		if(currentFrame > 12)
		{
			currentFrame = 0;
			i++;
		}
		
		if (i > 21)
		{
			for (i = 0; i <=12; i++)
			{
				oslDeleteImage(bootAnim[i]);
			}
			if (firstBoot == 1)
				displayLangSelection("system/settings/language");
			else 
				firstBootInitiation();
		}
    }
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	return 0;
}

int main()
{
	initOSLib(); //Initializes OsLib
	oslIntraFontInit(INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8); //Initializes OSL fonts

	//Loads our audio tones
	camera_click = oslLoadSoundFile("system/media/audio/ui/camera_click.wav", OSL_FMT_NONE);
	KeypressStandard = oslLoadSoundFile("system/media/audio/ui/KeypressStandard.wav", OSL_FMT_NONE);
	Lock = oslLoadSoundFile("system/media/audio/ui/Lock.wav", OSL_FMT_NONE);
	Unlock = oslLoadSoundFile("system/media/audio/ui/Unlock.wav", OSL_FMT_NONE);
	
	strcpy(backgroundPath, setFileDefaultsChar("system/settings/background.bin", "system/framework/framework-res/res/background.png", backgroundPath));
	strcpy(fontPath, setFileDefaultsChar("system/settings/font.bin", "system/fonts/Roboto.pgf", fontPath));
	
	hrTime = setFileDefaultsInt("system/app/clock/timeSet.bin", 0, hrTime);
	language = setFileDefaultsInt("system/settings/language.bin", 0, language);
	batteryM = setFileDefaultsInt("system/settings/battery.bin", 1, batteryM);
	fontSize = setFileDefaultsFloat("system/settings/fontSize.bin", 0.5, fontSize);
	
	checkGBootActivation();
	createDirs();
	themesLoad();
	
	quickSettings = oslLoadImageFile(quickSettingsBgPath, OSL_IN_VRAM, OSL_PF_8888);
	batt100 = oslLoadImageFile("system/home/icons/100.png", OSL_IN_VRAM, OSL_PF_8888);
	batt80 = oslLoadImageFile("system/home/icons/80.png", OSL_IN_VRAM, OSL_PF_8888);
	batt60 = oslLoadImageFile("system/home/icons/60.png", OSL_IN_VRAM, OSL_PF_8888);
	batt40 = oslLoadImageFile("system/home/icons/40.png", OSL_IN_VRAM, OSL_PF_8888);
	batt20 = oslLoadImageFile("system/home/icons/20.png", OSL_IN_VRAM, OSL_PF_8888);
	batt10 = oslLoadImageFile("system/home/icons/10.png", OSL_IN_VRAM, OSL_PF_8888);
	batt0 = oslLoadImageFile("system/home/icons/0.png", OSL_IN_VRAM, OSL_PF_8888);
	battcharge = oslLoadImageFile("system/home/icons/charge.png", OSL_IN_VRAM, OSL_PF_8888);
	volumeBar = oslLoadImageFilePNG("system/home/menu/volumeBar.png", OSL_IN_RAM, OSL_PF_8888);
	volumeControl = oslLoadImageFile("system/home/menu/volumeControl.png", OSL_IN_RAM, OSL_PF_8888);
	layerA = oslLoadImageFilePNG("system/home/icons/layerA.png", OSL_IN_RAM, OSL_PF_8888);
	layerB = oslLoadImageFilePNG("system/home/icons/layerB.png", OSL_IN_RAM, OSL_PF_8888);
	layerC = oslLoadImageFilePNG("system/home/icons/layerC.png", OSL_IN_RAM, OSL_PF_8888);
	layerD = oslLoadImageFilePNG("system/home/icons/layerD.png", OSL_IN_RAM, OSL_PF_8888);
	
	Roboto = oslLoadIntraFontFile(fontPath, INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslSetFont(Roboto); //Load and set font
	
	SceUID kModule, kModule1, kModule2;
	
	kModule = pspSdkLoadStartModule("modules/display.prx", PSP_MEMORY_PARTITION_KERNEL);
	kModule1 = pspSdkLoadStartModule("modules/control.prx", PSP_MEMORY_PARTITION_KERNEL);
	kModule2 = pspSdkLoadStartModule("modules/impose.prx", PSP_MEMORY_PARTITION_KERNEL);
	
	deleteUpdateFile(); //Delete update.zip
	
	setCpuBoot(); //Set default CPU or load pre-existing value
	setPowerManagement(); //Set default power save settings or load pre-existing values.
	
	getPSPNickname(); //Get PSP name before hand
	strcpy (pspname, nickname);
	
	//Main loop to run the program
	while (!osl_quit)
	{		
		bootAnimation();
		
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

