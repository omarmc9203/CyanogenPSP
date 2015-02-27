#include "apollo.h"
#include "home.h"
#include "clock.h"
#include "fm.h"
#include "lock.h"
#include "mp3player.h"
#include "multi.h"
#include "power_menu.h"
#include "screenshot.h"
#include "include/utils.h"
#include "id3.h"

int MP3Scan(const char* path )
{
	curScroll = 1;
	sprintf(lastDir, path);

	int i;
	for (i=0; i<=MAX_FILES; i++)	// erase old folders
		dirScan[i].exist = 0;

	int x;
	for (x=0; x<=MAX_FILES; x++) 
	{
		folderIcons[x].active = 0;
	}

	int fd = sceIoDopen(path);

	i = 1;
	
	if (fd) 
	{
		if (!(stricmp(path, "ms0:")==0 || (stricmp(path, "ms0:/")==0))) 
		{
			sceIoDread(fd, &g_dir);		// get rid of '.' and '..'
			sceIoDread(fd, &g_dir);

			// Create our own '..'
			folderIcons[1].active = 1; 
			sprintf(folderIcons[1].filePath, "doesn't matter");
			sprintf(folderIcons[1].name, "..");
			sprintf(folderIcons[1].fileType, "dotdot");

			x = 2;
		} 
		else 
		{
			x = 1;
		}
		
		while ( sceIoDread(fd, &g_dir) && i<=MAX_FILES ) 
		{
			sprintf( dirScan[i].name, g_dir.d_name );
			sprintf( dirScan[i].path, "%s/%s", path, dirScan[i].name );
			
			//skip . and .. entries
			if (!strcmp(".", g_dir.d_name) || !strcmp("..", g_dir.d_name)) 
			{
				memset(&g_dir, 0, sizeof(SceIoDirent));
				continue;
			};

			if (g_dir.d_stat.st_attr & FIO_SO_IFDIR) 
			{
				dirScan[i].directory = 1;
				dirScan[i].exist = 1;
			}
			else 
			{
				dirScan[i].directory = 0;
				dirScan[i].exist = 1;
			}

			dirScan[i].size = g_dir.d_stat.st_size;
			i++;
		}
	}

	sceIoDclose(fd);

	for (i=1; i<MAX_FILES; i++) {
		if (dirScan[i].exist == 0) 
		break;
		folderIcons[x].active = 1;
		sprintf(folderIcons[x].filePath, dirScan[i].path);
		sprintf(folderIcons[x].name, dirScan[i].name);

		char *suffix = strrchr(dirScan[i].name, '.');
		
		if (dirScan[i].directory == 1) 
		{      // if it's a directory
			sprintf(folderIcons[x].fileType, "fld");
		} 
		else if ((dirScan[i].directory == 0) && (suffix)) 
		{		// if it's not a directory
			sprintf(folderIcons[x].fileType, "none");
		}
		else if (!(suffix)) 
		{
			sprintf(folderIcons[x].fileType, "none");
		}
		x++;
	}

	return 1;
}

void mp3Up()
{
	current--; // Subtract a value from current so the ">" goes up
	if ((current <= curScroll-1) && (curScroll > 1)) 
	{
		curScroll--; // To do with how it scrolls
	}
}

void mp3Down()
{
	if (folderIcons[current+1].active) current++; // Add a value onto current so the ">" goes down
	if (current >= (MAX_MP3_DISPLAY+curScroll)) 
	{
		curScroll++; // To do with how it scrolls
	}
}

void mp3Upx5()
{
	current -= 5;  // Subtract a value from current so the ">" goes up
	if ((current <= curScroll-1) && (curScroll > 1)) {
		curScroll -= 5;  // To do with how it scrolls
	}
}

void mp3Downx5()
{
	if (folderIcons[current+1].active) current += 5; // Add a value onto current so the ">" goes down
	if (current >= (MAX_DISPLAY+curScroll)) {
		curScroll += 5; // To do with how it scrolls
	}
}
/*

char *compact_str(char *s, int max_length) {
	char *suffix;
	char t[max_length+1];
 
	if(strlen(s) > max_length) {
		suffix = strrchr(s, '.');
			if(suffix != NULL) {			
				strncpy(t, s, max_length-4);
				t[max_length-4] = '\0';
				s = strcat(t, suffix);   	
			} else {
				strncpy(t, s, max_length-1);
				t[max_length] = '\0';
				strcpy(s, t);
			}
	}

	return s;
}

void display_mp3_info(struct FILE_INFO *file) {
	
	int y_start = 25; //210


	if(file->cover != NULL)
		oslDrawImageXY(file->cover, 305, 23);  


	//oslDrawStringf(MP3DISPLAY_X, 190, "ID3Tag: %s", file->mp3Info.ID3.versionfound);	 	

	//oslDrawStringf(MP3DISPLAY_X, 200, "Title : %s", compact_str(file->mp3Info.ID3.ID3Title, 28));			 

	//oslDrawStringf(MP3DISPLAY_X, 210, "%s", compact_str(file->mp3Info.ID3.ID3Album, 28));			 

	//oslDrawStringf(MP3DISPLAY_X, 220, "Year  : %s", file->mp3Info.ID3.ID3Year);			 

	oslDrawStringf(250, 81, "%s", compact_str(file->mp3Info.ID3.ID3Artist, 28));			 

	//oslDrawStringf(MP3DISPLAY_X, 240, "Genre : %s", compact_str(file->mp3Info.ID3.ID3GenreText, 28));			 	
}
*/
/*

void getMP3METagInfo(char *filename, struct fileInfo *targetInfo){
    //ID3:
    struct ID3Tag ID3;
    ID3 = ParseID3(filename);
    strcpy(targetInfo->title, ID3.ID3Title);
    strcpy(targetInfo->artist, ID3.ID3Artist);
    strcpy(targetInfo->album, ID3.ID3Album);
    strcpy(targetInfo->year, ID3.ID3Year);
    strcpy(targetInfo->genre, ID3.ID3GenreText);
    strcpy(targetInfo->trackNumber, ID3.ID3TrackText);
    targetInfo->length = ID3.ID3Length / 1000;
}
*/

void MP3Play(char * path)
{	
	nowplaying = oslLoadImageFilePNG("system/app/apollo/nowplaying.png", OSL_IN_RAM, OSL_PF_8888);

	if (!nowplaying)
		debugDisplay();
	
	scePowerSetClockFrequency(333, 333, 166);
	
	pspAudioInit();
	
	int i;
	MP3_Init(1);
	MP3_Load(path);
	MP3_Play();
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();		
		
		oslClearScreen(RGB(0,0,0));

		oslReadKeys();
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,0);
		
		oslDrawImageXY(nowplaying, 0, 0);
		oslDrawStringf(250,76,folderIcons[current].name);
		//display_mp3_info(folderIcons[current].name);
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
		
		battery(370,2,1);
		digitaltime(420,4,458);
		
		if(osl_keys->pressed.select) 
		{
			oslDeleteImage(nowplaying);
			return;
		}
		
		else if(osl_keys->pressed.cross) 
		{
			MP3_Pause();
			for(i=0; i<10; i++) 
			{
				sceDisplayWaitVblankStart();
			}
		}
			
		if (MP3_EndOfStream() == 1) 
		{
			pspAudioEnd();
			MP3_Stop();
			releaseAudioCh();
			current++;
			MP3Play(folderIcons[current].filePath);
		}
		
		if(osl_keys->pressed.circle)
		{
			MP3_Pause();
			MP3_End();
			releaseAudioCh();
			oslDeleteImage(nowplaying);
			return;
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
	MP3_End();
}

void soundPlay(char * path)
{	
	nowplaying = oslLoadImageFilePNG("system/app/apollo/nowplaying.png", OSL_IN_RAM, OSL_PF_8888);
	
	OSL_SOUND * sound = oslLoadSoundFile(path, OSL_FMT_NONE);

	if (!nowplaying)
		debugDisplay();
	
	oslInitAudioME(3);
	
	oslPlaySound(sound,0);
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();		
		
		oslClearScreen(RGB(0,0,0));

		oslReadKeys();
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,0);
		
		oslDrawImageXY(nowplaying, 0, 0);
		oslDrawStringf(250,76,folderIcons[current].name);
		//display_mp3_info(folderIcons[current].name);
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
		
		battery(370,2,1);
		digitaltime(420,4,458);
		
		if(osl_keys->pressed.select) 
		{
			oslDeleteImage(nowplaying);
			return;
		}
		
		if(osl_keys->pressed.cross) 
		{
			oslPauseSound(sound,-1);
		}
		
		if(osl_keys->pressed.circle)
		{
			oslStopSound(sound);
			oslDeleteSound(sound);
			oslDeleteImage(nowplaying);
			return;
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
	oslStopSound(sound);
	oslDeleteSound(sound);
	oslDeleteImage(nowplaying);
	return 0;
}

void mp3FileDisplay()
{
	oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,0);

	oslDrawImageXY(mp3bg, 0, 0);
	oslDrawImageXY(mp3_select,8,(current - curScroll)*55+MP3_CURR_DISPLAY_Y);
	
	// Displays the directories, while also incorporating the scrolling
	for(i=curScroll;i<MAX_MP3_DISPLAY+curScroll;i++) 
	{
		char * ext = strrchr(dirScan[i].name, '.'); //For file extension.
	
		// Handles the cursor and the display to not move past the MAX_MP3_DISPLAY.
		// For moving down
		//if ((folderIcons[i].active == 0) && (current >= i-1)) {
	
		if ((folderIcons[i].active == 0) && (current >= i-1)) 
		{
			current = i-1;
			break;
		}
		// For moving up
		if (current <= curScroll-1) 
		{
			current = curScroll-1;
			break;
		}
		
		// If the currently selected item is active, then display the name
		if (folderIcons[i].active == 1)
		{	
			oslDrawStringf(MP3_DISPLAY_X, (i - curScroll)*55+MP3_DISPLAY_Y, folderIcons[i].name);	// change the X & Y value accordingly if you want to move it (for Y, just change the +10)		
		}
	}
	oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);

	battery(370,2,1);
	digitaltime(420,4,458);
}

void mp3Controls() //Controls
{
	oslReadKeys();	

	if (pad.Buttons != oldpad.Buttons) {
	
		if ((pad.Buttons & PSP_CTRL_DOWN) && (!(oldpad.Buttons & PSP_CTRL_DOWN))) 
		{
			mp3Down();
			timer = 0;
		}
		else if ((pad.Buttons & PSP_CTRL_UP) && (!(oldpad.Buttons & PSP_CTRL_UP))) 
		{
			mp3Up();
			timer = 0;
		}
		if ((pad.Buttons & PSP_CTRL_RIGHT) && (!(oldpad.Buttons & PSP_CTRL_RIGHT))) {
			mp3Downx5();
			timer = 0;
		}
		else if ((pad.Buttons & PSP_CTRL_LEFT) && (!(oldpad.Buttons & PSP_CTRL_LEFT))) {
			mp3Upx5();
			timer = 0;
		}
		if ((pad.Buttons & PSP_CTRL_TRIANGLE) && (!(oldpad.Buttons & PSP_CTRL_TRIANGLE))) 
		{
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
	
	if (osl_keys->pressed.circle)
	{		
			if(!strcmp("ms0:/MUSIC", lastDir)) 
			{
				oslDeleteImage(mp3bg);
				oslDeleteImage(mp3_select);
				oslDeleteFont(Roboto);
				appdrawer();
			}
			if(!strcmp("ms0:/PSP/MUSIC", lastDir)) 
			{
				oslDeleteImage(mp3bg);
				oslDeleteImage(mp3_select);
				oslDeleteFont(Roboto);
				appdrawer();
			}
			if(!strcmp("ms0:/PSP/GAME/CyanogenMod/downloads", lastDir)) 
			{
				oslDeleteImage(mp3bg);
				oslDeleteImage(mp3_select);
				oslDeleteFont(Roboto);
				appdrawer();
			}
			else
			{
				oslDeleteImage(mp3bg);
				oslDeleteImage(mp3_select);
				oslDeleteFont(Roboto);
				mp3player();
			}		
	}
	
	if (((ext) != NULL) && ((strcmp(ext ,".mp3") == 0) || ((strcmp(ext ,".MP3") == 0))) && (osl_keys->pressed.cross))
	{
		MP3Play(folderIcons[current].filePath);
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
	if ((timer > 30) && (pad.Buttons & PSP_CTRL_UP)) 
	{
		mp3Up();
		timer = 25;
	}
	else if ((timer > 30) && (pad.Buttons & PSP_CTRL_DOWN)) 
	{
		mp3Down();
		timer = 25;
	}

	if (current < 1) current = 1; // Stop the ">" from moving past the minimum files
	if (current > MAX_FILES) current = MAX_FILES; // Stop the ">" from moving past the max files

}

char * mp3Browse(const char * path)
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
		mp3FileDisplay();
		mp3Controls();
		
		sceDisplayWaitVblankStart();
		
		if (strlen(returnMe) > 4) 
		{
			break;
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();	
	}
	return returnMe;
}

void mp3View(char * browseDirectory)
{	
	mp3bg = oslLoadImageFilePNG("system/app/apollo/mp3bg.png", OSL_IN_RAM, OSL_PF_8888);
	mp3_select = oslLoadImageFilePNG("system/app/apollo/mp3_select.png", OSL_IN_RAM, OSL_PF_8888);
	
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,0);
	oslSetFont(Roboto);

	char * Directory = mp3Browse(browseDirectory);

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

int mp3player() 
{
	mp3bg = oslLoadImageFilePNG("system/app/apollo/mp3bg.png", OSL_IN_RAM, OSL_PF_8888);
	mp3_select = oslLoadImageFilePNG("system/app/apollo/mp3_select.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!mp3bg || !mp3_select)
		debugDisplay();
		
	Roboto = oslLoadIntraFontFile("system/fonts/Roboto.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslSetFont(Roboto);
	
	int MenuSelection = 1; // Pretty obvious
	int selector_x = 8; //The x position of the first selection
	int selector_y = 43; //The y position of the first selection
	int selector_image_x; //Determines the starting x position of the selection
	int selector_image_y = 55; //Determines the starting y position of the selection
	int numMenuItems = 3; //Amount of items in the menu
	int selection = 0;

	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		oslReadKeys();
		oslClearScreen(RGB(0,0,0));	
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,0);
		
		oslDrawImageXY(mp3bg, 0, 0);
		oslDrawImage(mp3_select);
		
		oslDrawStringf(20,108,"MUSIC");
		oslDrawStringf(20,163,"PSP/MUSIC");
		oslDrawStringf(20,218,"PSP/GAME/CyanogenMod/Downloads");
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
		
		battery(370,2,1);
		digitaltime(420,4,458);
		
		mp3_select->x = selector_image_x; //Sets the selection coordinates
        mp3_select->y = selector_image_y; //Sets the selection coordinates
        
        selector_image_x = selector_x+(mp3XSelection*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(mp3YSelection*MenuSelection); //Determines where the selection image is drawn for each selection
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last
		
		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {	
			oslDeleteImage(mp3bg);
			oslDeleteImage(mp3_select);
			oslDeleteFont(Roboto);
			mp3View("ms0:/MUSIC");
        }
		if (MenuSelection == 2 && osl_keys->pressed.cross)
        {		
			oslDeleteImage(mp3bg);
			oslDeleteImage(mp3_select);
			oslDeleteFont(Roboto);
			mp3View("ms0:/PSP/MUSIC");
        }
		if (MenuSelection == 3 && osl_keys->pressed.cross)
        {			
			oslDeleteImage(mp3bg);
			oslDeleteImage(mp3_select);
			oslDeleteFont(Roboto);
			mp3View("ms0:/PSP/GAME/CyanogenMod/downloads");
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(mp3bg);
			oslDeleteImage(mp3_select);
			oslDeleteFont(Roboto);
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
