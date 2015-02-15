#include <pspkernel.h>
#include <oslib/oslib.h>
#include <pspumd.h> 
#include <string.h>
#include <psploadexec.h> 
#include <psploadexec_kernel.h> 
#include "clock.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "gallery.h"
#include "screenshot.h"

OSL_IMAGE *background, *cursor, *music, *gmail, *messengericon, *browser, *google, *calc, *clockx, *email, *people, *calendar, *umd, 
		  *gallery, *fb, *settings,*pointer, *pointer1, *isoloadericon, *backicon, *homeicon, *multicon, *backdrop;

void appdrawer_loadImages()
{
	clockx = oslLoadImageFilePNG("system/home/icons/clock.png", OSL_IN_RAM, OSL_PF_8888);
	email = oslLoadImageFilePNG("system/home/icons/email.png", OSL_IN_RAM, OSL_PF_8888);
	fb = oslLoadImageFilePNG("system/home/icons/fb.png", OSL_IN_RAM, OSL_PF_8888);
	settings = oslLoadImageFilePNG("system/home/icons/settings.png", OSL_IN_RAM, OSL_PF_8888);
	gallery = oslLoadImageFilePNG("system/home/icons/gallery.png", OSL_IN_RAM, OSL_PF_8888);
	umd = oslLoadImageFilePNG("system/home/icons/umd.png", OSL_IN_RAM, OSL_PF_8888);
	calc = oslLoadImageFilePNG("system/home/icons/calc.png", OSL_IN_RAM, OSL_PF_8888);
	calendar = oslLoadImageFilePNG("system/home/icons/calendar.png", OSL_IN_RAM, OSL_PF_8888);
	people = oslLoadImageFilePNG("system/home/icons/people.png", OSL_IN_RAM, OSL_PF_8888);
	isoloadericon = oslLoadImageFilePNG("system/home/icons/isoloadericon.png", OSL_IN_RAM, OSL_PF_8888);
	backdrop = oslLoadImageFilePNG("system/home/icons/backdrop.png", OSL_IN_RAM, OSL_PF_8888);
}

void appdrawer_deleteImages()
{
	oslDeleteImage(clockx);
	oslDeleteImage(email);
	oslDeleteImage(fb);
	oslDeleteImage(settings);
	oslDeleteImage(gallery);
	oslDeleteImage(umd);
	oslDeleteImage(calc);
	oslDeleteImage(calendar);
	oslDeleteImage(people);
	oslDeleteImage(isoloadericon);
	oslDeleteImage(backdrop);
}

int appdrawer()
{	
	int music_x = 30;
	int browser_x = 93;
	int browser_text_x = 88;
	int calc_x = 156;
	int calc_text_x = 147;
	int calendar_x = 217;
	int calendar_text_x = 218;
	int clock_x = 277;
	int clock_text_x = 283;
	int email_x = 337;
	int email_text_x = 342;
	int fm_x = 397;
	int fm_text_x = 395;
	int gallery_text_x = 29;
	int gmail_x = 95;
	int gmail_text_x = 98;
	int game_icon = 155;
	int game_text_icon = 163;
	int messenger_x = 217;
	int messenger_text_x = 211; 
	int people_x = 277;
	int people_text_x = 279;
	int settings_x = 338;
	int settings_test_x = 335;
	int umd_x = 402;
	int umd_text_x = 406;

	//loads appdrawer icons
	appdrawer_loadImages();
	setfont();

	int highlight = 0;
	
	if (!clockx || !settings || !email || !gallery || !calc || !umd || !calendar || !people || !fb || !isoloadericon || !backdrop)
		debugDisplay();
		
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();

		controls();	

		oslDrawImage(background);
		oslDrawImageXY(backdrop, 0, 15);
		oslDrawImageXY(music, music_x, 35);
		oslDrawString(music_x,85,"Music");
		oslDrawImageXY(browser, browser_x, 35);
		oslDrawString(browser_text_x,85,"Browser");
		oslDrawImageXY(calc, calc_x, 35);
		oslDrawString(calc_text_x,85,"Calculator");
		oslDrawImageXY(calendar, calendar_x, 38);
		oslDrawString(calendar_text_x,85,"Calendar");
		oslDrawImageXY(clockx, clock_x, 35);
		oslDrawString(clock_text_x,85,"Clock");
		oslDrawImageXY(email, email_x, 35);
		oslDrawString(email_text_x,85,"Email");
		oslDrawImageXY(fb, fm_x, 35);
		oslDrawString(fm_text_x+10,85,"File");
		oslDrawString(fm_text_x,95,"Manager");
		oslDrawImageXY(gallery, music_x, 120);
		oslDrawString(gallery_text_x,175,"Gallery");
		oslDrawImageXY(gmail, gmail_x, 120);
		oslDrawString(gmail_text_x,175,"Gmail");
		oslDrawImageXY(isoloadericon, game_icon, 121);
		oslDrawString(game_text_icon,175,"Game");
		oslDrawImageXY(messengericon, messenger_x, 120);
		oslDrawString(messenger_text_x,175,"Messages");
		oslDrawImageXY(people, people_x, 120);
		oslDrawString(people_text_x,175,"People");
		oslDrawImageXY(settings, settings_x, 120);
		oslDrawString(settings_test_x,175,"Settings");
		oslDrawImageXY(umd, umd_x, 122);
		oslDrawString(umd_text_x,175,"UMD");
		
		digitaltime(420,4,458);
		battery(370,2,1);
		navbarButtons(1);
		
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
		
		androidQuickSettings();
		oslDrawImage(cursor);
			
		if (osl_pad.held.left && (osl_pad.held.cross))
		{
			music_x = music_x-10;
			browser_x = browser_x-10;
			browser_text_x = browser_text_x-10;
			calc_x = calc_x-10;
			calc_text_x = calc_text_x-10;
			calendar_x = calendar_x-10;
			calendar_text_x = calendar_text_x-10;
			clock_x = clock_x-10;
			clock_text_x = clock_text_x-10;
			email_x = email_x-10;
			email_text_x = email_text_x-10;
			fm_x = fm_x-10;
			fm_text_x = fm_text_x-10;
			gallery_text_x = gallery_text_x-10;
			gmail_x = gmail_x-10;
			gmail_text_x = gmail_text_x-10;
			game_icon = game_icon-10;
			game_text_icon = game_text_icon-10;
			messenger_x = messenger_x-10;
			messenger_text_x = messenger_text_x-10;
			people_x = people_x-10;
			people_text_x = people_text_x-10;
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
			calendar_x = calendar_x+10;
			calendar_text_x = calendar_text_x+10;
			clock_x = clock_x+10;
			clock_text_x = clock_text_x+10;
			email_x = email_x+10;
			email_text_x = email_text_x+10;
			fm_x = fm_x+10;
			fm_text_x = fm_text_x+10;
			gallery_text_x = gallery_text_x+10;
			gmail_x = gmail_x+10;
			gmail_text_x = gmail_text_x+10;
			game_icon = game_icon+10;
			game_text_icon = game_text_icon+10;
			messenger_x = messenger_x+10;
			messenger_text_x = messenger_text_x+10;
			people_x = people_x+10;
			people_text_x = people_text_x+10;
			settings_x = settings_x+10;
			settings_test_x = settings_test_x+10;
			umd_x = umd_x+10;
			umd_text_x = umd_text_x+10;
		}
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}

		if (cursor->x >= 78 && cursor->x <= 120 && cursor->y >= 24 && cursor->y <= 70 && osl_keys->pressed.cross)
		{
			appdrawer_deleteImages();
			internet();
		}
		
		if (cursor->x >= 96 && cursor->x <= 140 && cursor->y >= 118 && cursor->y <= 165 && osl_keys->pressed.cross) 
		{
			appdrawer_deleteImages();
			openGmail();
		}
		
		if (cursor->x >= 18 && cursor->x <= 65 && cursor->y >= 110 && cursor->y <= 155 && osl_keys->pressed.cross)
		{
			appdrawer_deleteImages();
			galleryApp();
		}
		
		if (osl_keys->pressed.L)
		{
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			appdrawer_deleteImages();
			home();
		}
		/* WIP Calculator
		if (cursor->x >= 142 && cursor->x <= 177 && cursor->y >= 44 && cursor->y <= 60 && osl_keys->pressed.cross)
		{
			appdrawer_deleteImages();
			calculator();
		}
		*/
		
		if (cursor->x >= 142 && cursor->x <= 177 && cursor->y >= 44 && cursor->y <= 60 && osl_keys->pressed.triangle)
		{
			highlight = 1;
			if (highlight == 1)
			{
			oslDrawImageXY(calc, cursor->x - 5, cursor->y - 5);
			}
		}
		
		if (cursor->x >= 335 && cursor->x <= 390 && cursor->y >= 119 && cursor->y <= 188 && osl_keys->pressed.cross)
		{
			appdrawer_deleteImages();
			settingsMenu();
		}
		
		if (cursor->x >= 395 && cursor->x <= 450 && cursor->y >= 119 && cursor->y <= 188 && osl_keys->pressed.cross)
		{
			int i;
			
			while(1) 
			{
				if(osl_keys->pressed.circle) 
				{
					break;
				}	 

				i = sceUmdCheckMedium();
				
				if (i == 0) 
				{ 
					oslDrawStringf(10,10,"UMD not inserted, returning home");
					oslSyncFrame();
					sceKernelDelayThread(2*1000000);
					home();
				}

				i = sceUmdActivate(1, "disc0:");
				oslDrawStringf(10,30,"Mounted UMD");
				i = sceUmdWaitDriveStat(UMD_WAITFORINIT);
				SceUID fd = sceIoOpen("disc0:/UMD_DATA.BIN", PSP_O_RDONLY, 0777);
				
				if(fd >= 0)
				{
					char game_id[11];
					sceIoRead(fd, game_id, 10);
					sceIoClose(fd);
					game_id[10] = 0;
					oslDrawStringf(10,50,"Found Game %s",game_id);
				}
					
				sceKernelLoadExec("disc0:/PSP_GAME/SYSDIR/BOOT.BIN",0);
				}
				
				/*
				void UMDBin() 
				{
					int fd;
					fd = sceIoOpen("disc0:/UMD_DATA.BIN", PSP_O_RDONLY, 0777);
					if(fd >= 0)
					{
						sceIoRead(fd, game_id, 10);
						sceIoClose(fd);
						game_id[10] = 0;
					}

					fd = sceIoDopen("disc0:/UMD_VIDEO/");
					if(fd >= 0) 
					{
						isvideo = 1;
					}
					if (isvideo == 1) 
					{
						skinc = sceIoOpen("disc0:/UMD_VIDEO/ICON0.PNG", PSP_O_RDONLY, 0); 
						
						if(skinc < 0) 
						{
							skin = 0;
						}
						else 
						{	
							skin = 1;
						}
						sceIoClose(skinc);
						
						if (skin == 1)
						{
							sprintf(iconbuf,"disc0:/UMD_VIDEO/ICON0.PNG");
							umdicon = loadImage(iconbuf);
						}
					}
					else 
					{
						skinc = sceIoOpen("disc0:/PSP_GAME/ICON0.PNG", PSP_O_RDONLY, 0); // check existence of game icon
						if(skinc < 0) 
						{
							skin = 0;
						}
						else 
						{
							skin = 1;
						}
						
						sceIoClose(skinc);
						
						if (skin == 1) 
						{
							sprintf(iconbuf,"disc0:/PSP_GAME/ICON0.PNG");
							umdicon = loadImage(iconbuf);
						}
					} // isvideo
				}
*/
		}
		
		if (cursor->x >= 18 && cursor->x <= 65 && cursor->y >= 25 && cursor->y <= 70 && osl_keys->pressed.cross)
		{
			appdrawer_deleteImages();
			mp3player();
		}
		
		/* Clock, MP3 Player Messenger and Game Menu
		
		if (cursor->x >= 266 && cursor->x <= 311 && cursor->y >= 25 && cursor->y <= 70 && osl_keys->pressed.cross)
		{
			appdrawer_deleteImages();
			pspclock();
		}
		
		
		if (cursor->x >= 210 && cursor->x <= 268 && cursor->y >= 118 && cursor->y <= 170 && osl_keys->pressed.cross)
		{
			appdrawer_deleteImages();
			messenger();
		}
		*/
		
		if (cursor->x >= 18 && cursor->x <= 65 && cursor->y >= 25 && cursor->y <= 70 && osl_keys->pressed.cross)
		{
			appdrawer_deleteImages();
			mp3player();
		}
		
		if (cursor->x >= 147 && cursor->x <= 191 && cursor->y >= 112 && cursor->y <= 154 && osl_keys->pressed.cross)
		{
			appdrawer_deleteImages();
			gameApp();
		}
		
		if (cursor->x >= 387 && cursor->x <= 432 && cursor->y >= 25 && cursor->y <= 70 && osl_keys->pressed.cross)
		{
			appdrawer_deleteImages();
			filemanage();
		}
		
		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			appdrawer_deleteImages();
			home();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			appdrawer_deleteImages();
			home();
		}
		
		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			multitask();
		}
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
				
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
	}
}

