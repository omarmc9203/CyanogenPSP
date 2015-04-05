#include "home.h"
#include "game.h"
#include "clock.h"
#include "fm.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "screenshot.h"
#include "settingsmenu.h"
#include "include/systemctrl_se.h"  

void gameUp()
{
	current--; // Subtract a value from current so the ">" goes up
	if ((current <= curScroll-1) && (curScroll > 1)) {
		curScroll--; // To do with how it scrolls
	}
}

void gameDown()
{
	if (folderIcons[current+1].active) current++; // Add a value onto current so the ">" goes down
	if (current >= (MAX_GAME_DISPLAY+curScroll)) {
		curScroll++; // To do with how it scrolls
	}
}

void gameUpx5()
{
	current -= 5;  // Subtract a value from current so the ">" goes up
	if ((current <= curScroll-1) && (curScroll > 1)) {
		curScroll -= 5;  // To do with how it scrolls
	}
}

void gameDownx5()
{
	if (folderIcons[current+1].active) current += 5; // Add a value onto current so the ">" goes down
	if (current >= (MAX_DISPLAY+curScroll)) {
		curScroll += 5; // To do with how it scrolls
	}
}

int launchEbootMs0(const char *path[])
{
	// Load Execute Parameter
	struct SceKernelLoadExecVSHParam param;

	// Clear Memory
	memset(&param, 0, sizeof(param));

	// Configure Parameters
	param.size = sizeof(param);
	param.args = strlen(path) + 1;
	param.argp = (void *)path;
	param.key = "game";

	// Trigger Reboot
	return sctrlKernelLoadExecVSHWithApitype(0x141, path, &param);
}

int launchEbootEf0(const char *path[])
{
	// Load Execute Parameter
	struct SceKernelLoadExecVSHParam param;

	// Clear Memory
	memset(&param, 0, sizeof(param));

	// Configure Parameters
	param.size = sizeof(param);
	param.args = strlen(path) + 1;
	param.argp = (void *)path;
	param.key = "game";

	// Trigger Reboot
	return sctrlKernelLoadExecVSHWithApitype(0x152, path, &param);
}

int launchPOPsMs0(const char *path[])
{
	// Load Execute Parameter
	struct SceKernelLoadExecVSHParam param;

	// Clear Memory
	memset(&param, 0, sizeof(param));

	// Configure Parameters
	param.size = sizeof(param);
	param.args = strlen(path) + 1;
    param.argp = (void *)path;
    param.key = "pops";

	// Trigger Reboot
	return sctrlKernelLoadExecVSHWithApitype(0x144, path, &param);
}

int launchPOPsEf0(const char *path[])
{
	// Load Execute Parameter
	struct SceKernelLoadExecVSHParam param;

	// Clear Memory
	memset(&param, 0, sizeof(param));

	// Configure Parameters
	param.size = sizeof(param);
	param.args = strlen(path) + 1;
    param.argp = (void *)path;
    param.key = "pops";

	// Trigger Reboot
	return sctrlKernelLoadExecVSHWithApitype(0x155, path, &param);
}

int launchISOMs0(const char *path[])
{
	char* bootpath = "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN";

	// Load Execute Parameter
	struct SceKernelLoadExecVSHParam param;
	
	// Clear Memory
	memset(&param, 0, sizeof(param));
	
	// Set Common Parameters
	param.size = sizeof(param);
	
	// EBOOT Path
    char * ebootpath = "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN";
               
    // Prepare ISO Reboot
    param.args = strlen(ebootpath) + 1;
    param.argp = ebootpath;
    param.key = "umdemu";
               
	// Enable Galaxy ISO Emulator Patch
    sctrlSESetBootConfFileIndex(MODE_INFERNO);
    sctrlSESetUmdFile(path);
	
	return sctrlKernelLoadExecVSHWithApitype(0x123, path, &param);
}

int launchISOEf0(const char *path[])
{
	char* bootpath = "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN";

	// Load Execute Parameter
	struct SceKernelLoadExecVSHParam param;
	
	// Clear Memory
	memset(&param, 0, sizeof(param));
	
	// Set Common Parameters
	param.size = sizeof(param);
	
	// EBOOT Path
    char * ebootpath = "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN";
               
    // Prepare ISO Reboot
    param.args = strlen(ebootpath) + 1;
    param.argp = ebootpath;
    param.key = "umdemu";
               
	// Enable Galaxy ISO Emulator Patch
    sctrlSESetBootConfFileIndex(MODE_INFERNO);
    sctrlSESetUmdFile(path);
	
	return sctrlKernelLoadExecVSHWithApitype(0x125, path, &param);
}

void gameDisplay()
{	
	oslDrawImageXY(gamebg, 0, 0);
	oslDrawString(10,10,"Game Launcher");
	oslDrawImageXY(gameSelection,0,(current - curScroll)*29+GAME_CURR_DISPLAY_Y);
	
	// Displays the directories, while also incorporating the scrolling
	for(i=curScroll;i<MAX_GAME_DISPLAY+curScroll;i++) {
	
		char * ext = strrchr(dirScan[i].name, '.'); //For file extension.
	
		// Handles the cursor and the display to not move past the MAX_GAME_DISPLAY.
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

		/*
			getIcon0(folderIcons[i].name);			 
			if(icon0!=NULL)
			{
				oslDrawImageXY(icon0, 327,100);
			}
		*/
		
		// If the currently selected item is active, then display the name
		if (folderIcons[i].active == 1) {
			
			oslDrawStringf(GAME_DISPLAY_X, (i - curScroll)*29+GAME_DISPLAY_Y, folderIcons[i].name);	// change the X & Y value accordingly if you want to move it (for Y, just change the +10)
		}
	}
}

void gameControls(int n) //Controls
{
	oslReadKeys();	

	if (pad.Buttons != oldpad.Buttons) {
	
		if ((pad.Buttons & PSP_CTRL_DOWN) && (!(oldpad.Buttons & PSP_CTRL_DOWN))) {
			gameDown();
			timer = 0;
		}
		else if ((pad.Buttons & PSP_CTRL_UP) && (!(oldpad.Buttons & PSP_CTRL_UP))) {
			gameUp();
			timer = 0;
		}
		
		if ((pad.Buttons & PSP_CTRL_RIGHT) && (!(oldpad.Buttons & PSP_CTRL_RIGHT))) {
			gameDownx5();
			timer = 0;
		}
		else if ((pad.Buttons & PSP_CTRL_LEFT) && (!(oldpad.Buttons & PSP_CTRL_LEFT))) {
			gameUpx5();	
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
	
	if (n == 0) //For regular eboots
	{
		if (((ext) != NULL) && ((strcmp(ext ,".PBP") == 0) || (strcmp(ext ,".pbp") == 0)) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1); 
			if (kuKernelGetModel() == 4)
			{
				if(gBootActivator == 1)
				{
					gameBoot();
					launchEbootEf0(folderIcons[current].filePath);
				}
				else if(gBootActivator == 0)
				{
					launchEbootEf0(folderIcons[current].filePath);
				}
			}
			else
			{
				if(gBootActivator == 1)
				{
					gameBoot();
					launchEbootMs0(folderIcons[current].filePath);
				}
				else if(gBootActivator == 0)
				{
					launchEbootMs0(folderIcons[current].filePath);
				}
			}
		}
	}
	
	else if (n == 1) //For POPS
	{ 
		if (((ext) != NULL) && ((strcmp(ext ,".PBP") == 0) || (strcmp(ext ,".pbp") == 0)) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1); 
			if (kuKernelGetModel() == 4)
			{
				if(gBootActivator == 1)
				{
					gameBoot();
					launchPOPsEf0(folderIcons[current].filePath);
				}
				else if(gBootActivator == 0)
				{
					launchPOPsEf0(folderIcons[current].filePath);
				}
			}
			else
			{
				if(gBootActivator == 1)
				{
					gameBoot();
					launchPOPsMs0(folderIcons[current].filePath);
				}
				else if(gBootActivator == 0)
				{
					launchPOPsMs0(folderIcons[current].filePath);
				}
			}
		}
	}
	
	if (((ext) != NULL) && ((strcmp(ext ,".iso") == 0) || ((strcmp(ext ,".ISO") == 0)) || ((strcmp(ext ,".cso") == 0)) || ((strcmp(ext ,".CSO") == 0))) && (osl_keys->held.cross))
	{ 
		if (kuKernelGetModel() == 4)
			{
				if(gBootActivator == 1)
				{
					gameBoot();
					launchISOEf0(folderIcons[current].filePath);
				}
				else if(gBootActivator == 0)
				{
					launchISOEf0(folderIcons[current].filePath);
				}
			}
			else
			{
				if(gBootActivator == 1)
				{
					gameBoot();
					launchISOMs0(folderIcons[current].filePath);
				}
				else if(gBootActivator == 0)
				{
					launchISOMs0(folderIcons[current].filePath);
				}
			}
	}
	
	if (osl_keys->pressed.circle)
	{		
		if(!(strcmp("ms0:/PSP/GAME", lastDir))) 
		{
			dirBack();
		}
		if(!(strcmp("ms0:/ISO", lastDir)))
		{
			dirBack();
		}
		else
		{
			gameUnload();
			gameApp();
		}	
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
		gameUp();
		timer = 25;
	} else if ((timer > 30) && (pad.Buttons & PSP_CTRL_DOWN)) {
		gameDown();
		timer = 25;
	}

	if (current < 1) current = 1; // Stop the ">" from moving past the minimum files
	if (current > MAX_FILES) current = MAX_FILES; // Stop the ">" from moving past the max files

}

// Just call 'path' with whichever path you want it to start off in
char * gameBrowse(const char * path)
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
		gameDisplay();
		gameControls(0);
		
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

char * popsBrowse(const char * path)
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
		gameDisplay();
		gameControls(1);
		
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

void gameUnload()
{
	oslDeleteImage(gamebg);
	oslDeleteImage(gameSelection);
}

const OSL_VIRTUALFILENAME __image_ram_files[] = {
	{"ram:/Media/icon0.png", (void*)icon0_png, sizeof(icon0_png), &VF_MEMORY}
};

void getIcon0(char* filename){
    //unsigned char _header[40];
    int icon0Offset, icon1Offset;
    char file[256];
    sprintf(file,"%s/eboot.pbp",filename);
    SceUID fd = sceIoOpen(file, 0x0001/*O_RDONLY*/, 0777);
	if(fd < 0){
		icon0 = oslLoadImageFilePNG("ram:/Media/icon0.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
		return;
	}
    //sceIoRead(fd, _header, 40);
    //printf("letto header\n");
    sceIoLseek(fd, 12, SEEK_SET);
    sceIoRead(fd, &icon0Offset, 4);
    //sceIoLseek(fd, 23, SEEK_SET);
    sceIoRead(fd, &icon1Offset, 4);
    int icon0_size = icon1Offset - icon0Offset;
    sceIoLseek(fd, icon0Offset, SEEK_SET);
    unsigned char icon[icon0_size];
    if(icon0_size){
        sceIoRead(fd, icon, icon0_size);
        oslSetTempFileData(icon, icon0_size, &VF_MEMORY);
        icon0 = oslLoadImageFilePNG(oslGetTempFileName(), OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
    }else{
        icon0 = oslLoadImageFilePNG("ram:/Media/icon0.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
    }
    sceIoClose(fd);
}

void gameView(char * browseDirectory, int type)
{	
	char * Directory;

	gamebg = oslLoadImageFilePNG("system/app/game/gamebg.png", OSL_IN_RAM, OSL_PF_8888);
	gameSelection = oslLoadImageFilePNG("system/app/game/gameselector.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);

	if (type == 0)
	{
		Directory = gameBrowse(browseDirectory); //For PSP Eboots
	}
	
	else if (type == 1)
	{
		Directory = popsBrowse(browseDirectory); //For POPS Eboots
	}
	
	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		oslClearScreen(RGB(0,0,0));	
		
		centerText(480/2, 272/2, Directory, 50);	// Shows the path that 'Directory' was supposed to receive from mp3Browse();
	 
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
	}	
	return 0;
}

void checkGBootActivation()
{
	FILE * gBootActivation;

	if (!(fileExists("system/app/game/boot/gBootActivator.txt")))
	{
		gBootActivation = fopen("system/app/game/boot/gBootActivator.txt", "w");
		fprintf(gBootActivation, "1");
		fclose(gBootActivation);
	}

	gBootActivation = fopen("system/app/game/boot/gBootActivator.txt", "r");
	fscanf(gBootActivation,"%d",&gBootActivator);
	fclose(gBootActivation);
}

void gameBoot()
{
	int currentFrame = 0, i = 0;

    gameAnim[0] = oslLoadImageFilePNG("system/app/game/boot/0.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[1] = oslLoadImageFilePNG("system/app/game/boot/1.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[2] = oslLoadImageFilePNG("system/app/game/boot/2.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[3] = oslLoadImageFilePNG("system/app/game/boot/3.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[4] = oslLoadImageFilePNG("system/app/game/boot/4.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[5] = oslLoadImageFilePNG("system/app/game/boot/5.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[6] = oslLoadImageFilePNG("system/app/game/boot/6.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[7] = oslLoadImageFilePNG("system/app/game/boot/7.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[8] = oslLoadImageFilePNG("system/app/game/boot/8.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);

    while(!osl_quit)
	{
		oslStartDrawing();
        oslDrawImageXY(gameAnim[currentFrame], 0, 0); 
		sceDisplayWaitVblankStart();
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
		sceKernelDelayThread(10000 * 6);
   
		currentFrame++;
		if(currentFrame > 8)
		{
			i++;
			currentFrame = 0;
		}
		
		if (i > 2)
		{	
			while(!osl_quit)
			{
				oslStartDrawing();
				oslDrawImageXY(gameAnim[0], 0, 0); 
				oslEndDrawing();
				oslEndFrame();
				oslSyncFrame();
				sceKernelDelayThread(3000000);
				return;
			}
		}
	}
}

int gameApp() 
{
	gamebg = oslLoadImageFilePNG("system/app/game/gamebg.png", OSL_IN_RAM, OSL_PF_8888);
	gameSelection = oslLoadImageFilePNG("system/app/game/gameselector.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	if (!gamebg || !gameSelection)
		debugDisplay();
		
	checkGBootActivation();
	
	int MenuSelection = 1; // Pretty obvious
	int selector_x = 0; //The x position of the first selection
	int selector_y = 45; //The y position of the first selection
	int selector_image_x; //Determines the starting x position of the selection
	int selector_image_y = 0; //Determines the starting y position of the selection
	int numMenuItems = 3; //Amount of items in the menu
	int selection = 0;

	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		oslReadKeys();
		oslClearScreen(RGB(0,0,0));	
		oslDrawImageXY(gamebg, 0, 0);
		oslIntraFontSetStyle(Roboto, 0.5, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
		oslDrawString(10,10,"Game Launcher");
		oslDrawImage(gameSelection);
		
		oslDrawStringf(10,79,"GAME");
		oslDrawStringf(10,108,"ISO/CSO");
		oslDrawStringf(10,137,"POPS");
		
		gameSelection->x = selector_image_x; //Sets the selection coordinates
        gameSelection->y = selector_image_y; //Sets the selection coordinates
        
        selector_image_x = selector_x+(game_xSelection*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(game_ySelection*MenuSelection); //Determines where the selection image is drawn for each selection
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last
		
		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {		
			oslPlaySound(KeypressStandard, 1);  
			gameUnload();
			gameView("ms0:/PSP/GAME", 0); //PSP Homebrew
        }
		if (MenuSelection == 2 && osl_keys->pressed.cross)
        {		
			oslPlaySound(KeypressStandard, 1);  
			gameUnload();
			gameView("ms0:/ISO", 0); //ISO backups
        }
		if (MenuSelection == 3 && osl_keys->pressed.cross)
        {		
			oslPlaySound(KeypressStandard, 1);  
			gameUnload();
			gameView("ms0:/PSP/GAME", 1); //POPS
        }

		if (osl_keys->pressed.circle)
		{
			gameUnload();
			appdrawer();
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
		
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
	}	
	return selection;
}
