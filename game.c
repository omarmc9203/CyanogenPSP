#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <psppower.h>
#include <string.h>
#include <stdio.h>
#include <oslib/oslib.h>
#include "game.h"
#include "clock.h"
#include "fm.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "screenshot.h"

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
		if(((ext) != NULL) && ((strcmp(ext ,".PBP") == 0)) && (dirScan[i].directory == 0)) 
		{
			//
		}
		*/
		
		// If the currently selected item is active, then display the name
		if (folderIcons[i].active == 1) {
			
			oslDrawStringf(GAME_DISPLAY_X, (i - curScroll)*29+GAME_DISPLAY_Y, folderIcons[i].name);	// change the X & Y value accordingly if you want to move it (for Y, just change the +10)
		}
	}
}

void gameControls() //Controls
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
		if ((pad.Buttons & PSP_CTRL_TRIANGLE) && (!(oldpad.Buttons & PSP_CTRL_TRIANGLE))) {
			if (!(stricmp(lastDir, "ms0:")==0) || (stricmp(lastDir, "ms0:/")==0)) {
				curScroll = 1;
				current = 1;
			}
		}
		if ((pad.Buttons & PSP_CTRL_CROSS) && (!(oldpad.Buttons & PSP_CTRL_CROSS))) {
			openDir(folderIcons[current].filePath, folderIcons[current].fileType);
		}
	}
	
	char * ext = strrchr(folderIcons[current].filePath, '.');
	
	if (((ext) != NULL) && ((strcmp(ext ,".PBP") == 0) || (strcmp(ext ,".pbp") == 0)) && (osl_keys->pressed.cross))
	{
		if (kuKernelGetModel() == 4)
			launchEbootEf0(folderIcons[current].filePath);
		else
			launchEbootMs0;
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
		gameControls();
		
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

void gameView(char * browseDirectory)
{	
	gamebg = oslLoadImageFilePNG("system/app/game/gamebg.png", OSL_IN_RAM, OSL_PF_8888);
	gameSelection = oslLoadImageFilePNG("system/app/game/gameselector.png", OSL_IN_RAM, OSL_PF_8888);

	char * Directory = gameBrowse(browseDirectory);

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

int gameApp() 
{
	gamebg = oslLoadImageFilePNG("system/app/game/gamebg.png", OSL_IN_RAM, OSL_PF_8888);
	gameSelection = oslLoadImageFilePNG("system/app/game/gameselector.png", OSL_IN_RAM, OSL_PF_8888);
		
	pgfFont = oslLoadFontFile("system/fonts/DroidSans.pgf");
	oslIntraFontSetStyle(pgfFont, 0.5, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	oslSetFont(pgfFont);
	
	if (!gamebg || !gameSelection)
		debugDisplay();
	
	int MenuSelection = 1; // Pretty obvious
	int selector_x = 0; //The x position of the first selection
	int selector_y = 45; //The y position of the first selection
	int selector_image_x; //Determines the starting x position of the selection
	int selector_image_y = 0; //Determines the starting y position of the selection
	int numMenuItems = 2; //Amount of items in the menu
	int selection = 0;

	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		oslReadKeys();
		oslClearScreen(RGB(0,0,0));	
		oslDrawImageXY(gamebg, 0, 0);
		oslDrawString(10,10,"Game Launcher");
		oslDrawImage(gameSelection);
		
		oslDrawStringf(10,79,"GAME");
		oslDrawStringf(10,108,"ISO/CSO");
		
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
			gameUnload();
			gameView("ms0:/PSP/GAME");
        }
		if (MenuSelection == 2 && osl_keys->pressed.cross)
        {		
			gameUnload();
			gameView("ms0:/ISO");
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
