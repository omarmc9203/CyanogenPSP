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
	backdrop = oslLoadImageFilePNG("system/home/icons/backdrop.png", OSL_IN_RAM, OSL_PF_8888);
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
		if (cursor->x >= 10 && cursor->x <= 75 && cursor->y >= 25 && cursor->y <= 90)
		{
			ic_launcher_browser->stretchX = ic_launcher_browser->sizeX * zoomIn;
			ic_launcher_browser->stretchY = ic_launcher_browser->sizeY * zoomOut;
		}
		
		else
		{
			ic_launcher_browser->stretchX = ic_launcher_browser->sizeX;
			ic_launcher_browser->stretchY = ic_launcher_browser->sizeY;
		}
		
		if (cursor->x >= 75 && cursor->x <= 140 && cursor->y >= 25 && cursor->y <= 90)
		{
			ic_launcher_calculator->stretchX = ic_launcher_calculator->sizeX * zoomIn;
			ic_launcher_calculator->stretchY = ic_launcher_calculator->sizeY * zoomOut;
		}
		
		else
		{
			ic_launcher_calculator->stretchX = ic_launcher_calculator->sizeX;
			ic_launcher_calculator->stretchY = ic_launcher_calculator->sizeY;
		}
		
		if (cursor->x >= 140 && cursor->x <= 205 && cursor->y >= 25 && cursor->y <= 90)
		{
			ic_launcher_clock->stretchX = ic_launcher_clock->sizeX * zoomIn;
			ic_launcher_clock->stretchY = ic_launcher_clock->sizeY * zoomOut;
		}
		
		else
		{
			ic_launcher_clock->stretchX = ic_launcher_clock->sizeX;
			ic_launcher_clock->stretchY = ic_launcher_clock->sizeY;
		}
		
		if (cursor->x >= 205 && cursor->x <= 270 && cursor->y >= 25 && cursor->y <= 90)
		{
			ic_launcher_filemanager->stretchX = ic_launcher_filemanager->sizeX * zoomIn;
			ic_launcher_filemanager->stretchY = ic_launcher_filemanager->sizeY * zoomOut;
		}
		
		else
		{
			ic_launcher_filemanager->stretchX = ic_launcher_filemanager->sizeX;
			ic_launcher_filemanager->stretchY = ic_launcher_filemanager->sizeY;
		}
		
		if (cursor->x >= 270 && cursor->x <= 335 && cursor->y >= 25 && cursor->y <= 90)
		{
			ic_launcher_gallery->stretchX = ic_launcher_gallery->sizeX * zoomIn;
			ic_launcher_gallery->stretchY = ic_launcher_gallery->sizeY * zoomOut;
		}
		
		else
		{
			ic_launcher_gallery->stretchX = ic_launcher_gallery->sizeX;
			ic_launcher_gallery->stretchY = ic_launcher_gallery->sizeY;
		}
		
		if (cursor->x >= 335 && cursor->x <= 400 && cursor->y >= 25 && cursor->y <= 90)
		{
			ic_launcher_game->stretchX = ic_launcher_game->sizeX * zoomIn;
			ic_launcher_game->stretchY = ic_launcher_game->sizeY * zoomOut;
		}
		
		else
		{
			ic_launcher_game->stretchX = ic_launcher_game->sizeX;
			ic_launcher_game->stretchY = ic_launcher_game->sizeY;
		}
		
		if (cursor->x >= 400 && cursor->x <= 465 && cursor->y >= 25 && cursor->y <= 90)
		{
			ic_launcher_messenger->stretchX = ic_launcher_messenger->sizeX * zoomIn;
			ic_launcher_messenger->stretchY = ic_launcher_messenger->sizeY * zoomOut;
		}
		
		else
		{
			ic_launcher_messenger->stretchX = ic_launcher_messenger->sizeX;
			ic_launcher_messenger->stretchY = ic_launcher_messenger->sizeY;
		}
		
		if (cursor->x >= 10 && cursor->x <= 75 && cursor->y >= 110 && cursor->y <= 175)
		{
			ic_launcher_apollo->stretchX = ic_launcher_apollo->sizeX * zoomIn;
			ic_launcher_apollo->stretchY = ic_launcher_apollo->sizeY * zoomOut;
		}
		
		else
		{
			ic_launcher_apollo->stretchX = ic_launcher_apollo->sizeX;
			ic_launcher_apollo->stretchY = ic_launcher_apollo->sizeY;
		}
		
		if (cursor->x >= 75 && cursor->x <= 140 && cursor->y >= 110 && cursor->y <= 175)
		{
			ic_launcher_settings->stretchX = ic_launcher_settings->sizeX * zoomIn;
			ic_launcher_settings->stretchY = ic_launcher_settings->sizeY * zoomOut;
		}
		
		else
		{
			ic_launcher_settings->stretchX = ic_launcher_settings->sizeX;
			ic_launcher_settings->stretchY = ic_launcher_settings->sizeY;
		}
		
		if (cursor->x >= 140 && cursor->x <= 205 && cursor->y >= 110 && cursor->y <= 175)
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
	int browser_x = 20;
	int browser_text_x = 42;
	int calc_x = 85;
	int calc_text_x = 111;
	int clock_x = 150;
	int clock_text_x = 174;
	int fm_x = 215;
	int fm_text_x = 240;
	int gallery_x = 280;
	int gallery_text_x = 304;
	int game_icon = 345;
	int game_text_icon = 368;
	int messenger_x = 410;
	int messenger_text_x = 430; 
	int music_x = 20;
	int music_text_x = 42;
	int settings_x = 85;
	int settings_test_x = 107;
	int umd_x = 150;
	int umd_text_x = 170;

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
		oslDrawImageXY(backdrop, 0, 15);
		oslDrawImageXY(ic_launcher_browser, browser_x, 35);
		oslDrawStringf(browser_text_x,85, "%s", lang_appDrawer[language][0]);
		oslDrawImageXY(ic_launcher_calculator, calc_x, 35);
		oslDrawStringf(calc_text_x,85, "%s", lang_appDrawer[language][1]);
		oslDrawImageXY(ic_launcher_clock, clock_x, 35);
		oslDrawStringf(clock_text_x,85, "%s", lang_appDrawer[language][2]);
		oslDrawImageXY(ic_launcher_filemanager, fm_x, 35);
		oslDrawStringf(fm_text_x,85,"%s", lang_appDrawer[language][3]);
		oslDrawImageXY(ic_launcher_gallery, gallery_x, 35);
		oslDrawStringf(gallery_text_x,85, "%s", lang_appDrawer[language][4]);
		oslDrawImageXY(ic_launcher_game, game_icon, 35);
		oslDrawStringf(game_text_icon,85, "%s", lang_appDrawer[language][5]);
		oslDrawImageXY(ic_launcher_messenger, messenger_x, 35);
		oslDrawStringf(messenger_text_x,85, "%s", lang_appDrawer[language][6]);
		oslDrawImageXY(ic_launcher_apollo, music_x, 120);
		oslDrawStringf(music_text_x,175, "%s", lang_appDrawer[language][7]);
		oslDrawImageXY(ic_launcher_settings, settings_x, 120);
		oslDrawStringf(settings_test_x,175, "%s", lang_appDrawer[language][8]);
		oslDrawImageXY(ic_launcher_umd, umd_x, 122);
		oslDrawStringf(umd_text_x,175, "%s", lang_appDrawer[language][9]);
		
		if (umd_x <= -10 && fm_x <= -10)
		{
			oslDrawImageXY(pointer1, 221, 223);
			oslDrawImageXY(pointer1, 232, 223);
			oslDrawImageXY(pointer, 243, 223);
		}
		else if (music_x >= 500)
		{
			oslDrawImageXY(pointer, 221, 223);
			oslDrawImageXY(pointer1, 232, 223);
			oslDrawImageXY(pointer1, 243, 223);
		}
		else
		{
			oslDrawImageXY(pointer1, 221, 223);
			oslDrawImageXY(pointer, 232, 223);
			oslDrawImageXY(pointer1, 243, 223);
		}
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
		androidQuickSettings();
		volumeController();
		appHighlight(1);
		oslDrawImage(cursor);
			
		if (osl_pad.held.left && (osl_pad.held.cross))
		{
			music_x = music_x-10;
			browser_x = browser_x-10;
			browser_text_x = browser_text_x-10;
			calc_x = calc_x-10;
			calc_text_x = calc_text_x-10;
			clock_x = clock_x-10;
			clock_text_x = clock_text_x-10;
			fm_x = fm_x-10;
			fm_text_x = fm_text_x-10;
			gallery_text_x = gallery_text_x-10;
			game_icon = game_icon-10;
			game_text_icon = game_text_icon-10;
			messenger_x = messenger_x-10;
			messenger_text_x = messenger_text_x-10;
			settings_x = settings_x-10;
			settings_test_x = settings_test_x-10;
			umd_x = umd_x-10;
			umd_text_x = umd_text_x-10;
		}
		
		else if (osl_pad.held.right && (osl_pad.held.cross))
		{
			music_x = music_x+10;
			browser_x = browser_x+10;
			browser_text_x = browser_text_x+10;
			calc_x = calc_x+10;
			calc_text_x = calc_text_x+10;
			clock_x = clock_x+10;
			clock_text_x = clock_text_x+10;
			fm_x = fm_x+10;
			fm_text_x = fm_text_x+10;
			gallery_text_x = gallery_text_x+10;
			game_icon = game_icon+10;
			game_text_icon = game_text_icon+10;
			messenger_x = messenger_x+10;
			messenger_text_x = messenger_text_x+10;
			settings_x = settings_x+10;
			settings_test_x = settings_test_x+10;
			umd_x = umd_x+10;
			umd_text_x = umd_text_x+10;
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
		
		if (osl_keys->pressed.circle)
		{
			appdrawer_deleteImages();
			home();
		}
		
		if (cursor->x >= 10 && cursor->x <= 75 && cursor->y >= 25 && cursor->y <= 90 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			internet();
		}
		
		/*// WIP Calculator
		if (cursor->x >= 75 && cursor->x <= 140 && cursor->y >= 25 && cursor->y <= 90 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			calculator();
		}*/
		
		if (cursor->x >= 140 && cursor->x <= 205 && cursor->y >= 25 && cursor->y <= 90 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			pspclock();
		}
		
		if (cursor->x >= 205 && cursor->x <= 270 && cursor->y >= 25 && cursor->y <= 90 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			appdrawer_deleteImages();
			filemanage();
		}
		
		if (cursor->x >= 270 && cursor->x <= 335 && cursor->y >= 25 && cursor->y <= 90 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			galleryApp();
		}
		
		if (cursor->x >= 335 && cursor->x <= 400 && cursor->y >= 25 && cursor->y <= 90 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			gameApp();
		}

		/*
		if (cursor->x >= 400 && cursor->x <= 465 && cursor->y >= 25 && cursor->y <= 90 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			messenger();
		}
		*/
		
		if (cursor->x >= 10 && cursor->x <= 75 && cursor->y >= 110 && cursor->y <= 175 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			mp3player();
		}
		
		if (cursor->x >= 75 && cursor->x <= 140 && cursor->y >= 110 && cursor->y <= 175 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1); 
			appdrawer_deleteImages();
			settingsMenu();
		}
		
		if (cursor->x >= 140 && cursor->x <= 205 && cursor->y >= 110 && cursor->y <= 175 && osl_keys->pressed.cross)
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
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			oslPlaySound(KeypressStandard, 1); 
			screenshot();
		}
				
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
	}
	return 0;
}

