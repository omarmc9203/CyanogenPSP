#include "musicPlayer.h"
#include "appDrawer.h"
#include "homeMenu.h"
#include "clock.h"
#include "fileManager.h"
#include "lockScreen.h"
#include "recentsMenu.h"
#include "powerMenu.h"
#include "screenshot.h"
#include "settingsMenu.h"
#include "include/audio/mp3playerME.h"
#include "include/utils.h"

int imposeGetVolume();

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
		if (!(strcmp(path, "ms0:")==0 || (strcmp(path, "ms0:/")==0))) 
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

void MP3Play(char * path)
{	
	struct ID3Tag ID3;

	nowplaying = oslLoadImageFilePNG(nowplayingBgPath, OSL_IN_RAM, OSL_PF_8888);
	mp3Play = oslLoadImageFilePNG("system/app/apollo/play.png", OSL_IN_RAM, OSL_PF_8888);
	mp3Pause = oslLoadImageFilePNG("system/app/apollo/pause.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!nowplaying)
		debugDisplay();
	
	scePowerSetClockFrequency(333, 333, 166);
	
	pspAudioInit();
	
	int i, mp3Min = 0;
	MP3ME_Init(1);
	ParseID3(path, &ID3);
	MP3ME_Load(path);
	MP3ME_Play();
	
	isPlaying = 1;
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();		
		
		oslClearScreen(RGB(0,0,0));

		oslReadKeys();
		
		oslIntraFontSetStyle(Roboto, fontSize, BLACK, 0, 0);
		
		if (MP3ME_playingTime > 59)
		{
			mp3Min += 1;
			MP3ME_playingTime = 0;
		}
		oslDrawImageXY(nowplaying, 0, 0);
		oslDrawStringf(240,76, "Playing: %.19s", folderIcons[current].name);
		oslDrawStringf(240,96, "Title: %.21s", ID3.ID3Title);
		
		oslDrawStringf(240,116, "Artist: %.20s", ID3.ID3Artist);
		oslDrawStringf(240,136, "Album: %.21s", ID3.ID3Album);
		oslDrawStringf(240,156, "Year: %.22s", ID3.ID3Year);
		oslDrawStringf(240,176, "Genre: %.21s", ID3.ID3GenreText);
		oslDrawStringf(435,206, "0%d:%.f", mp3Min, MP3ME_playingTime);
		
		if (MP3ME_isPlaying == 1)
			oslDrawImageXY(mp3Play, 230, 224);
		if (MP3ME_isPlaying == 0)
			oslDrawImageXY(mp3Pause, 230, 224);
		
		battery(370,2,1);
		digitaltime(420,4,0,hrTime);
		volumeController();
		
		if(osl_keys->pressed.select) 
		{
			oslDeleteImage(nowplaying);
			oslDeleteImage(mp3Play);
			oslDeleteImage(mp3Pause);
			return;
		}
		
		if(MP3ME_isPlaying == 1 && osl_keys->pressed.cross) 
		{
			oslPlaySound(KeypressStandard, 1); 
			MP3ME_Pause();
			for(i=0; i<10; i++) 
			{
				sceDisplayWaitVblankStart();
			}
		}
		
		else if (MP3ME_isPlaying == 0 && osl_keys->pressed.cross)
		{
			MP3ME_Play();
		}
			
		if (MP3ME_EndOfStream() == 1) 
		{
			MP3ME_Stop();
		}
		
		if(osl_keys->pressed.circle)
		{
			endAudioLib();
			MP3ME_Stop();
			releaseAudio();
			oslDeleteImage(nowplaying);
			oslDeleteImage(mp3Play);
			oslDeleteImage(mp3Pause);
			isPlaying = 0;
			setCpuBoot(); //Restore previous CPU state
			return;
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
	
		captureScreenshot();
		
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
		}
}

int soundPlay(char * path)
{	
	nowplaying = oslLoadImageFilePNG(nowplayingBgPath, OSL_IN_RAM, OSL_PF_8888);
	mp3Play = oslLoadImageFilePNG("system/app/apollo/play.png", OSL_IN_RAM, OSL_PF_8888);
	mp3Pause = oslLoadImageFilePNG("system/app/apollo/pause.png", OSL_IN_RAM, OSL_PF_8888);
	
	OSL_SOUND * sound = oslLoadSoundFile(path, OSL_FMT_NONE);

	if (!nowplaying)
		debugDisplay();
	
	oslInitAudioME(3);
	
	oslPlaySound(sound,0);
	
	isPlaying = 1;
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();		
		
		oslClearScreen(RGB(0,0,0));

		oslReadKeys();
		
		oslIntraFontSetStyle(Roboto, fontSize, BLACK, 0, 0);
		
		oslDrawImageXY(nowplaying, 0, 0);
		oslDrawImageXY(mp3Play, 230, 224);
		oslDrawStringf(240,76, "%.28s", folderIcons[current].name);
		
		battery(370,2,1);
		digitaltime(420,4,0,hrTime);
		volumeController();
		
		if(osl_keys->pressed.select) 
		{
			oslDeleteImage(nowplaying);
			oslDeleteImage(mp3Play);
			oslDeleteImage(mp3Pause);
			return 1;
		}
		
		if(osl_keys->pressed.cross) 
		{
			oslPlaySound(KeypressStandard, 1); 
			oslPauseSound(sound,-1);
		}
		
		if(osl_keys->pressed.circle)
		{
			isPlaying = 0;
			oslStopSound(sound);
			oslDeleteSound(sound);
			oslDeleteImage(nowplaying);
			oslDeleteImage(mp3Play);
			oslDeleteImage(mp3Pause);
			return 1;
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
	
		captureScreenshot();
		
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
		}
	oslStopSound(sound);
	oslDeleteSound(sound);
	oslDeleteImage(nowplaying);
	oslDeleteImage(mp3Play);
	oslDeleteImage(mp3Pause);
	return 0;
}

void mp3FileDisplay()
{
	oslIntraFontSetStyle(Roboto, fontSize, BLACK, 0, 0);

	oslDrawImageXY(mp3bg, 0, 0);
	oslDrawImageXY(mp3_select,8,(current - curScroll)*55+MP3_CURR_DISPLAY_Y);
	
	// Displays the directories, while also incorporating the scrolling
	for(i=curScroll;i<MAX_MP3_DISPLAY+curScroll;i++) 
	{
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
			oslDrawStringf(MP3_DISPLAY_X, (i - curScroll)*55+MP3_DISPLAY_Y, "%.68s", folderIcons[i].name);	// change the X & Y value accordingly if you want to move it (for Y, just change the +10)		
		}
	}

	battery(370,2,1);
	digitaltime(420,4,0,hrTime);
}

void mp3Controls() //Controls
{
	oslReadKeys();	
	
	if (pad.Buttons != oldpad.Buttons) 
	{
		if (osl_keys->pressed.down) 
		{
			mp3Down();
			timer = 0;
		}
		else if (osl_keys->pressed.up) 
		{
			mp3Up();
			timer = 0;
		}
	
		if (osl_keys->pressed.right) 
		{
			mp3Downx5();
			timer = 0;
		}
		else if (osl_keys->pressed.left) 
		{
			mp3Upx5();
			timer = 0;
		}
	
		if (osl_keys->pressed.triangle) 
		{
			curScroll = 1;
			current = 1;
		}
	
		if (osl_keys->pressed.cross) 
		{
			oslPlaySound(KeypressStandard, 1); 
			openDir(folderIcons[current].filePath, folderIcons[current].fileType);
		}
	}
	
	volumeController();
	
	char * ext = strrchr(folderIcons[current].filePath, '.'); 
	
	if (osl_keys->pressed.circle)
	{			
		if((strcmp("ms0:/MUSIC", lastDir)==0) || (strcmp("ms0:/PSP/MUSIC", lastDir)==0) || (strcmp("ms0:/PSP/GAME/CyanogenPSP/downloads", lastDir)==0) || (strcmp("ms0:/", lastDir)==0))
		{
			oslDeleteImage(mp3bg);
			oslDeleteImage(mp3_select);
			mp3player();
		}
		else if((strcmp("ms0:/MUSIC", lastDir)!=0)) 
		{
			dirBack(3);
		}
		else if((strcmp("ms0:/PSP/MUSIC", lastDir)!=0))
		{
			dirBack(4);
		}	
		else if((strcmp("ms0:/PSP/GAME/CyanogenPSP/downloads", lastDir)!=0))
		{
			dirBack(5);
		}	
	}
	
	if (((ext) != NULL) && ((strcmp(ext ,".mp3") == 0) || ((strcmp(ext ,".MP3") == 0))) && (osl_keys->pressed.cross))
	{
		if (isPlaying == 1)
		{
			endAudioLib();
			MP3ME_Stop();
			releaseAudio();
			MP3Play(folderIcons[current].filePath);
		}
		else 
		{
			oslPlaySound(KeypressStandard, 1); 
			MP3Play(folderIcons[current].filePath);
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
	
	captureScreenshot();
	
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

	if (current < 1) 
		current = 1;
	if (current > MAX_FILES) 
		current = MAX_FILES;

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
		
		if (strlen(returnMe) > 4) 
			break;	
			
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();	
	}
	return returnMe;
}

int mp3View(char * browseDirectory)
{	
	mp3bg = oslLoadImageFilePNG(apolloBgPath, OSL_IN_RAM, OSL_PF_8888);
	mp3_select = oslLoadImageFilePNG(apolloSelectorPath, OSL_IN_RAM, OSL_PF_8888);

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
	mp3bg = oslLoadImageFilePNG(apolloBgPath, OSL_IN_RAM, OSL_PF_8888);
	mp3_select = oslLoadImageFilePNG(apolloSelectorPath, OSL_IN_RAM, OSL_PF_8888);
	
	if (!mp3bg || !mp3_select)
		debugDisplay();

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
		
		selector_image_x = selector_x+(mp3XSelection*MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y+(mp3YSelection*MenuSelection); //Determines where the selection image is drawn for each selection
	
		oslStartDrawing();
		oslReadKeys();
		oslClearScreen(RGB(0,0,0));	
		
		oslIntraFontSetStyle(Roboto, fontSize, BLACK, 0, 0);
		
		oslDrawImageXY(mp3bg, 0, 0);
		oslDrawImageXY(mp3_select, selector_image_x, selector_image_y);
		
		oslDrawStringf(20,108,"MUSIC");
		oslDrawStringf(20,163,"PSP/MUSIC");
		oslDrawStringf(20,218,"PSP/GAME/CyanogenPSP/Downloads");
		
		battery(370,2,1);
		digitaltime(420,4,0,hrTime);
		volumeController();
		
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last
		
		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(mp3bg);
			oslDeleteImage(mp3_select);
			mp3View("ms0:/MUSIC");
        }
		else if (MenuSelection == 2 && osl_keys->pressed.cross)
        {		
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(mp3bg);
			oslDeleteImage(mp3_select);
			mp3View("ms0:/PSP/MUSIC");
        }
		else if (MenuSelection == 3 && osl_keys->pressed.cross)
        {	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(mp3bg);
			oslDeleteImage(mp3_select);
			mp3View("ms0:/PSP/GAME/CyanogenPSP/downloads");
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(mp3bg);
			oslDeleteImage(mp3_select);
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
	
		captureScreenshot();
		
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
	}	
	return selection;
}
