#include "home.h"
#include "gallery.h"
#include "appdrawer.h"
#include "fm.h"
#include "include/utils.h"

void galleryUp()
{
	current--; // Subtract a value from current so the ">" goes up
	if ((current <= curScroll-1) && (curScroll > 1)) {
		curScroll--; // To do with how it scrolls
	}
}

void galleryDown()
{
	if (folderIcons[current+1].active) current++; // Add a value onto current so the ">" goes down
	if (current >= (MAX_GALLERY_DISPLAY+curScroll)) {
		curScroll++; // To do with how it scrolls
	}
}

void galleryDisplay()
{	
	oslDrawImageXY(gallerybg, 0, 0);
	oslDrawImageXY(gallerySelection,15,(current - curScroll)*56+GALLERY_CURR_DISPLAY_Y);
	oslDrawStringf(25,18,"Album");
	
	battery(370,2,1);
	digitaltime(420,4,458);
	
	// Displays the directories, while also incorporating the scrolling
	for(i=curScroll;i<MAX_GALLERY_DISPLAY+curScroll;i++) {
	
		char * ext = strrchr(dirScan[i].name, '.'); //For file extension.
	
		// Handles the cursor and the display to not move past the MAX_GALLERY_DISPLAY.
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

		oslDrawImageXY(galleryThumbnail,20,(i - curScroll)*56+THUMBNAIL_DISPLAY_Y);
		
		// If the currently selected item is active, then display the name
		if (folderIcons[i].active == 1) {
			
			oslDrawStringf(GALLERY_DISPLAY_X, (i - curScroll)*55+GALLERY_DISPLAY_Y, folderIcons[i].name);	// change the X & Y value accordingly if you want to move it (for Y, just change the +10)		
		}
	}
}

int changeWallpaper()
{
	wallpaper = oslLoadImageFilePNG("system/app/gallery/wallpaper.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!wallpaper)
		debugDisplay();

	while (!osl_quit) {
	
	oslStartDrawing();

	controls();	
	
	oslIntraFontSetStyle(Roboto, 0.5f, BLACK, 0, INTRAFONT_ALIGN_LEFT);
	
	oslDrawImageXY(wallpaper, 65, 67);
	oslDrawStringf(110,95,"Set picture as wallpaper?");
	oslDrawStringf(110,115,"Press (X) to accept or (O) to cancel.");
	
	if (osl_keys->pressed.cross) 
	{
		FILE * backgroundPathTXT = fopen("system/framework/framework-res/res/background.txt", "w");
		fprintf(backgroundPathTXT,"%s", folderIcons[current].filePath);
		fclose(backgroundPathTXT);
		oslDeleteImage(wallpaper);
		return;
	}
		
	if (osl_keys->pressed.circle) 
	{
		oslDeleteImage(wallpaper);
		return;
	}
		
	oslEndDrawing(); 
	oslEndFrame(); 
	oslSyncFrame();
	}
}

void showImage(const char * path)
{
	int zoomIn = 0, zoomOut = 0;

	OSL_IMAGE * image = oslLoadImageFile(path, OSL_IN_RAM, OSL_PF_8888);
	OSL_IMAGE * galleryBar = oslLoadImageFilePNG("system/app/gallery/galleryBar.png", OSL_IN_RAM, OSL_PF_8888);
	 
	if(!image)
		return 0;
		
	if (!galleryBar)
		debugDisplay();
	
	//Draw image in the centre
	oslSetImageRotCenter(image);
	image->x = OSL_SCREEN_WIDTH / 2;
	image->y = OSL_SCREEN_HEIGHT / 2;
		
	while (!osl_quit) {

		oslReadKeys();
		oslStartDrawing();	
		
		oslClearScreen(RGB(255,255,255));
		oslDrawImage(image);//draw image
		oslDrawImageXY(galleryBar,0,0);
		oslDrawStringf(40,12,folderIcons[current].name);
		
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();	
		
		if(osl_keys->analogY <= -50)
		{
			image->stretchX = image->sizeX * zoomIn++;
			image->stretchY = image->sizeY * zoomOut++;
		}
		
		if(osl_keys->analogY >= 50)
		{
			image->stretchX = image->sizeX * zoomIn--;
			image->stretchY = image->sizeY * zoomOut--;
		}
		
		if (osl_keys->pressed.square) 
		{
			changeWallpaper();
		}
		
		if (osl_keys->pressed.circle) 
		{
			oslDeleteImage(image);
			oslDeleteImage(galleryBar);
			return;
		}
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
	}
	//delete image
	oslDeleteImage(image);	
}

void galleryControls() //Controls
{
	oslReadKeys();	

	if (pad.Buttons != oldpad.Buttons) {
	
		if ((pad.Buttons & PSP_CTRL_DOWN) && (!(oldpad.Buttons & PSP_CTRL_DOWN))) {
			galleryDown();
			timer = 0;
		}
		else if ((pad.Buttons & PSP_CTRL_UP) && (!(oldpad.Buttons & PSP_CTRL_UP))) {
			galleryUp();
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
	
	if (osl_keys->pressed.cross)
	{
		showImage(folderIcons[current].filePath);
	}
	
	if (osl_keys->pressed.circle)
	{		
			if(!strcmp("ms0:/PICTURE", lastDir)) 
			{
				galleryUnload();
				appdrawer();
			}
			if(!strcmp("ms0:/PSP/PHOTO", lastDir)) 
			{
				galleryUnload();
				appdrawer();
			}
			if(!strcmp("ms0:/PSP/GAME/CyanogenMod/screenshots", lastDir)) 
			{
				galleryUnload();
				appdrawer();
			}
			else
			{
				galleryUnload();
				galleryApp();
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
		galleryUp();
		timer = 25;
	} else if ((timer > 30) && (pad.Buttons & PSP_CTRL_DOWN)) {
		galleryDown();
		timer = 25;
	}

	if (current < 1) current = 1; // Stop the ">" from moving past the minimum files
	if (current > MAX_FILES) current = MAX_FILES; // Stop the ">" from moving past the max files

}

// Just call 'path' with whichever path you want it to start off in
char * galleryBrowse(const char * path)
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
		galleryDisplay();
		galleryControls();
		
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

void galleryUnload()
{
	oslDeleteImage(gallerybg);
	oslDeleteImage(gallerySelection);
	oslDeleteImage(galleryThumbnail);
	oslDeleteFont(Roboto);
}

void galleryView(char * browseDirectory)
{	
	gallerybg = oslLoadImageFilePNG("system/app/gallery/gallerybg.png", OSL_IN_RAM, OSL_PF_8888);
	gallerySelection = oslLoadImageFilePNG("system/app/gallery/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	galleryThumbnail = oslLoadImageFilePNG("system/app/gallery/ic_images.png", OSL_IN_RAM, OSL_PF_8888);
	
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto-Regular.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslIntraFontSetStyle(Roboto, 0.5f, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	oslSetFont(Roboto);

	char * Directory = galleryBrowse(browseDirectory);

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

int galleryApp() 
{
	gallerybg = oslLoadImageFilePNG("system/app/gallery/gallerybg.png", OSL_IN_RAM, OSL_PF_8888);
	gallerySelection = oslLoadImageFilePNG("system/app/gallery/highlight.png", OSL_IN_RAM, OSL_PF_8888);
	galleryThumbnail = oslLoadImageFilePNG("system/app/gallery/ic_images.png", OSL_IN_RAM, OSL_PF_8888);
		
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto-Regular.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslIntraFontSetStyle(Roboto, 0.5f, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	oslSetFont(Roboto);
	
	if (!gallerybg || !gallerySelection || !galleryThumbnail)
		debugDisplay();
	
	int MenuSelection = 1; // Pretty obvious
	int selector_x = 15; //The x position of the first selection
	int selector_y = 10; //The y position of the first selection
	int selector_image_x; //Determines the starting x position of the selection
	int selector_image_y = 26; //Determines the starting y position of the selection
	int numMenuItems = 3; //Amount of items in the menu
	int selection = 0;

	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		oslReadKeys();
		oslClearScreen(RGB(0,0,0));	
		oslDrawImageXY(gallerybg, 0, 0);
		oslDrawStringf(25,18,"Album");
		oslDrawImage(gallerySelection);
		
		oslDrawStringf(25,89,"PICTURE");
		oslDrawStringf(25,145,"PSP/PHOTO");
		oslDrawStringf(25,201,"PSP/GAME/CyanogenPSP/screenshots");
		
		battery(370,2,1);
		digitaltime(420,4,458);
		
		gallerySelection->x = selector_image_x; //Sets the selection coordinates
        gallerySelection->y = selector_image_y; //Sets the selection coordinates
        
        selector_image_x = selector_x+(galley_xSelection*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(galley_ySelection*MenuSelection); //Determines where the selection image is drawn for each selection
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last
		
		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {		
			galleryUnload();
			galleryView("ms0:/PICTURE");
        }
		if (MenuSelection == 2 && osl_keys->pressed.cross)
        {		
			galleryUnload();
			galleryView("ms0:/PSP/PHOTO");
        }
		if (MenuSelection == 3 && osl_keys->pressed.cross)
        {			
			galleryUnload();
			galleryView("ms0:/PSP/GAME/CyanogenPSP/screenshots");
        }
		
		if (osl_keys->pressed.circle)
		{
			galleryUnload();
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
