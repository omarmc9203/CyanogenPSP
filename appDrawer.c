#include "appDrawer.h"
#include "language.h"
#include "calculator.h"
#include "musicPlayer.h"
#include "messenger.h"
#include "fileManager.h"
#include "gameLauncher.h"
#include "settingsMenu.h"
#include "clock.h"
#include "homeMenu.h"
#include "lockScreen.h"
#include "recentsMenu.h"
#include "powerMenu.h"
#include "gallery.h"
#include "screenshot.h"
#include "include/utils.h"

void appdrawer_loadImages()
{
	backdrop = oslLoadImageFilePNG(backdropPath, OSL_IN_RAM, OSL_PF_8888);
	ic_launcher_clock = oslLoadImageFilePNG(clockPath, OSL_IN_RAM, OSL_PF_8888);
}

void appdrawer_deleteImages()
{
	oslDeleteImage(backdrop);
	oslDeleteImage(ic_launcher_clock);
}

void appHighlight(int n)
{
	float zoomIn = 1.1, zoomOut = 1.1;

	if (n == 0)
	{	
		if (cursor->x >= 100 && cursor->x <= 154 && cursor->y >= 195 && cursor->y <= 240)
		{
			ic_launcher_apollo->stretchX = ic_launcher_apollo->sizeX * zoomIn;
			ic_launcher_apollo->stretchY = ic_launcher_apollo->sizeY * zoomOut;
		}
		else
		{
			ic_launcher_apollo->stretchX = ic_launcher_apollo->sizeX;
			ic_launcher_apollo->stretchY = ic_launcher_apollo->sizeY;
		}
	
		if (cursor->x >= 155 && cursor->x <= 210 && cursor->y >= 195 && cursor->y <= 240)
		{
			ic_launcher_messenger->stretchX = ic_launcher_messenger->sizeX * zoomIn;
			ic_launcher_messenger->stretchY = ic_launcher_messenger->sizeY * zoomOut;
		}
		else
		{
			ic_launcher_messenger->stretchX = ic_launcher_messenger->sizeX;
			ic_launcher_messenger->stretchY = ic_launcher_messenger->sizeY;
		}
	
		if (cursor->x >= 276 && cursor->x <= 321 && cursor->y >= 195 && cursor->y <= 240)
		{
			ic_launcher_browser->stretchX = ic_launcher_browser->sizeX * zoomIn;
			ic_launcher_browser->stretchY = ic_launcher_browser->sizeY * zoomOut;
		}
		else
		{
			ic_launcher_browser->stretchX = ic_launcher_browser->sizeX;
			ic_launcher_browser->stretchY = ic_launcher_browser->sizeY;
		}
		
		if (cursor->x >= 330 && cursor->x <= 374 && cursor->y >= 190 && cursor->y <= 240)
		{
			ic_launcher_settings->stretchX = ic_launcher_settings->sizeX * zoomIn;
			ic_launcher_settings->stretchY = ic_launcher_settings->sizeY * zoomOut;
		}
		else
		{
			ic_launcher_settings->stretchX = ic_launcher_settings->sizeX;
			ic_launcher_settings->stretchY = ic_launcher_settings->sizeY;
		}
	}

	else if (n == 1)
	{
		if (cursor->x >= 15 && cursor->x <= 75 && cursor->y >= 25 && cursor->y <= 85)
		{
			ic_launcher_browser->stretchX = ic_launcher_browser->sizeX * zoomIn;
			ic_launcher_browser->stretchY = ic_launcher_browser->sizeY * zoomOut;
		}
		else
		{
			ic_launcher_browser->stretchX = ic_launcher_browser->sizeX;
			ic_launcher_browser->stretchY = ic_launcher_browser->sizeY;
		}
		
		if (cursor->x >= 100 && cursor->x <= 140 && cursor->y >= 25 && cursor->y <= 85)
		{
			ic_launcher_calculator->stretchX = ic_launcher_calculator->sizeX * zoomIn;
			ic_launcher_calculator->stretchY = ic_launcher_calculator->sizeY * zoomOut;
		}
		else
		{
			ic_launcher_calculator->stretchX = ic_launcher_calculator->sizeX;
			ic_launcher_calculator->stretchY = ic_launcher_calculator->sizeY;
		}
		
		if (cursor->x >= 160 && cursor->x <= 225 && cursor->y >= 25 && cursor->y <= 85)
		{
			ic_launcher_clock->stretchX = ic_launcher_clock->sizeX * zoomIn;
			ic_launcher_clock->stretchY = ic_launcher_clock->sizeY * zoomOut;
		}
		else
		{
			ic_launcher_clock->stretchX = ic_launcher_clock->sizeX;
			ic_launcher_clock->stretchY = ic_launcher_clock->sizeY;
		}
		
		if (cursor->x >= 245 && cursor->x <= 295 && cursor->y >= 25 && cursor->y <= 85)
		{
			ic_launcher_filemanager->stretchX = ic_launcher_filemanager->sizeX * zoomIn;
			ic_launcher_filemanager->stretchY = ic_launcher_filemanager->sizeY * zoomOut;
		}
		else
		{
			ic_launcher_filemanager->stretchX = ic_launcher_filemanager->sizeX;
			ic_launcher_filemanager->stretchY = ic_launcher_filemanager->sizeY;
		}
		
		if (cursor->x >= 320 && cursor->x <= 370 && cursor->y >= 25 && cursor->y <= 85)
		{
			ic_launcher_gallery->stretchX = ic_launcher_gallery->sizeX * zoomIn;
			ic_launcher_gallery->stretchY = ic_launcher_gallery->sizeY * zoomOut;
		}
		else
		{
			ic_launcher_gallery->stretchX = ic_launcher_gallery->sizeX;
			ic_launcher_gallery->stretchY = ic_launcher_gallery->sizeY;
		}
		
		if (cursor->x >= 390 && cursor->x <= 450 && cursor->y >= 25 && cursor->y <= 85)
		{
			ic_launcher_game->stretchX = ic_launcher_game->sizeX * zoomIn;
			ic_launcher_game->stretchY = ic_launcher_game->sizeY * zoomOut;
		}
		else
		{
			ic_launcher_game->stretchX = ic_launcher_game->sizeX;
			ic_launcher_game->stretchY = ic_launcher_game->sizeY;
		}
		
		if (cursor->x >= 15 && cursor->x <= 75 && cursor->y >= 115 && cursor->y <= 180)
		{
			ic_launcher_messenger->stretchX = ic_launcher_messenger->sizeX * zoomIn;
			ic_launcher_messenger->stretchY = ic_launcher_messenger->sizeY * zoomOut;
		}
		else
		{
			ic_launcher_messenger->stretchX = ic_launcher_messenger->sizeX;
			ic_launcher_messenger->stretchY = ic_launcher_messenger->sizeY;
		}
		
		if (cursor->x >= 100 && cursor->x <= 140 && cursor->y >= 115 && cursor->y <= 180)
		{
			ic_launcher_apollo->stretchX = ic_launcher_apollo->sizeX * zoomIn;
			ic_launcher_apollo->stretchY = ic_launcher_apollo->sizeY * zoomOut;
		}
		else
		{
			ic_launcher_apollo->stretchX = ic_launcher_apollo->sizeX;
			ic_launcher_apollo->stretchY = ic_launcher_apollo->sizeY;
		}
		
		if (cursor->x >= 160 && cursor->x <= 225 && cursor->y >= 115 && cursor->y <= 180)
		{
			ic_launcher_settings->stretchX = ic_launcher_settings->sizeX * zoomIn;
			ic_launcher_settings->stretchY = ic_launcher_settings->sizeY * zoomOut;
		}
		else
		{
			ic_launcher_settings->stretchX = ic_launcher_settings->sizeX;
			ic_launcher_settings->stretchY = ic_launcher_settings->sizeY;
		}
		
		if (cursor->x >= 245 && cursor->x <= 295 && cursor->y >= 115 && cursor->y <= 180)
		{
			ic_launcher_umd->stretchX = ic_launcher_umd->sizeX * zoomIn;
			ic_launcher_umd->stretchY = ic_launcher_umd->sizeY * zoomOut;
		}
		else
		{
			ic_launcher_umd->stretchX = ic_launcher_umd->sizeX;
			ic_launcher_umd->stretchY = ic_launcher_umd->sizeY;
		}
	}
}

int appdrawer()
{	
	int browser_x = 30;
	int browser_text_x = 52;
	int calc_x = 105;
	int calc_text_x = 127;
	int clock_x = 180;
	int clock_text_x = 202;
	int fm_x = 255;
	int fm_text_x = 277;
	int gallery_x = 330;
	int gallery_text_x = 352;
	int game_icon = 405;
	int game_text_icon = 427;
	int messenger_x = 30;
	int messenger_text_x = 52; 
	int music_x = 105;
	int music_text_x = 127;
	int settings_x = 180;
	int settings_test_x = 202;
	int umd_x = 255;
	int umd_text_x = 277;

	//loads appdrawer icons
	appdrawer_loadImages();
	
	if (!ic_launcher_clock || !backdrop)
		debugDisplay();

	oslSetFont(Roboto);
		
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();

		controls();	
		
		oslDrawImage(background);
		
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
			navbarButtons(0);
			battery(370,2,1);
			digitaltime(420,4,0,hrTime);
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,INTRAFONT_ALIGN_CENTER);
		oslDrawImageXY(backdrop, 1, 15);
		oslDrawImageXY(ic_launcher_browser, browser_x, 40);
		oslDrawStringf(browser_text_x, 90, "%s", lang_appDrawer[language][0]);
		oslDrawImageXY(ic_launcher_calculator, calc_x, 40);
		oslDrawStringf(calc_text_x, 90, "%s", lang_appDrawer[language][1]);
		oslDrawImageXY(ic_launcher_clock, clock_x, 40);
		oslDrawStringf(clock_text_x, 90, "%s", lang_appDrawer[language][2]);
		oslDrawImageXY(ic_launcher_filemanager, fm_x, 40);
		oslDrawStringf(fm_text_x, 90,"%s", lang_appDrawer[language][3]);
		oslDrawImageXY(ic_launcher_gallery, gallery_x, 40);
		oslDrawStringf(gallery_text_x, 90, "%s", lang_appDrawer[language][4]);
		oslDrawImageXY(ic_launcher_game, game_icon, 40);
		oslDrawStringf(game_text_icon, 90, "%s", lang_appDrawer[language][5]);
		oslDrawImageXY(ic_launcher_messenger, messenger_x, 130);
		oslDrawStringf(messenger_text_x, 185, "%s", lang_appDrawer[language][6]);
		oslDrawImageXY(ic_launcher_apollo, music_x, 130);
		oslDrawStringf(music_text_x, 185, "%s", lang_appDrawer[language][7]);
		oslDrawImageXY(ic_launcher_settings, settings_x, 130);
		oslDrawStringf(settings_test_x, 185, "%s", lang_appDrawer[language][8]);
		oslDrawImageXY(ic_launcher_umd, umd_x, 132);
		oslDrawStringf(umd_text_x, 185, "%s", lang_appDrawer[language][9]);

		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
		androidQuickSettings();
		volumeController();
		appHighlight(1);
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{ 
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			appdrawer_deleteImages();
			home();
		}
		
		if (cursor->x >= 15 && cursor->x <= 75 && cursor->y >= 25 && cursor->y <= 85 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			internet();
		}
		
		/*// WIP Calculator
		if (cursor->x >= 100 && cursor->x <= 140 && cursor->y >= 25 && cursor->y <= 85 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			calculator();
		}*/
		
		if (cursor->x >= 160 && cursor->x <= 225 && cursor->y >= 25 && cursor->y <= 85 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			pspclock();
		}
		
		if (cursor->x >= 245 && cursor->x <= 295 && cursor->y >= 25 && cursor->y <= 85 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			appdrawer_deleteImages();
			filemanage();
		}
		
		if (cursor->x >= 320 && cursor->x <= 370 && cursor->y >= 25 && cursor->y <= 85 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			galleryApp();
		}
		
		if (cursor->x >= 390 && cursor->x <= 450 && cursor->y >= 25 && cursor->y <= 85 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			gameApp();
		}

		/*
		if (cursor->x >= 15 && cursor->x <= 75 && cursor->y >= 115 && cursor->y <= 180 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			messenger();
		}
		*/
		
		if (cursor->x >= 100 && cursor->x <= 140 && cursor->y >= 115 && cursor->y <= 180 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			mp3player();
		}
		
		if (cursor->x >= 160 && cursor->x <= 225 && cursor->y >= 115 && cursor->y <= 180 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			settingsMenu();
		}
		
		if (cursor->x >= 245 && cursor->x <= 295 && cursor->y >= 115 && cursor->y <= 180 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			launchUMD("disc0:/PSP_GAME/SYSDIR/EBOOT.BIN");
		}
		
		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			home();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			home();
		}
		
		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			multitask();
		}
		
		captureScreenshot();
				
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
	}
	return 0;
}

