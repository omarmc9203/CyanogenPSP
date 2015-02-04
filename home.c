#include <pspkernel.h>
#include <oslib/oslib.h>
#include "clock.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "screenshot.h"

OSL_IMAGE *background, *cursor, *ic_allapps, *ic_allapps_pressed, *music, *gmail, *messengericon, *browser, *google, *pointer, *pointer1, 
		  *backicon, *homeicon, *multicon;

void homeUnloadResources()
{
	oslDeleteImage(ic_allapps);
	oslDeleteImage(ic_allapps_pressed);
}

void home()
{	
	ic_allapps = oslLoadImageFilePNG("system/framework/framework-res/res/drawable-hdpi/ic_allapps.png", OSL_IN_RAM, OSL_PF_8888);
	ic_allapps_pressed = oslLoadImageFile("system/framework/framework-res/res/drawable-hdpi/ic_allapps_pressed.png", OSL_IN_RAM, OSL_PF_8888);

	setfont();

	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();

		controls();	
		
		oslDrawImage(background);	
		oslDrawImageXY(music, 105, 190);
		oslDrawImageXY(browser, 276, 190);
		oslDrawImageXY(gmail, 331, 190);
		oslDrawImageXY(messengericon, 160, 190);
		oslDrawImageXY(pointer, 232, 180);
				
		digitaltime(420,4,458);
		appDrawerIcon();
		battery(370,2,1);
		navbarButtons(1);
		androidQuickSettings();
		oslDrawImage(cursor);
			
		if (osl_keys->pressed.square)
		{
			powermenu();
		}

		if (cursor->x >= 276 && cursor->x <= 321 && cursor->y >= 195 && cursor->y <= 240 && osl_keys->pressed.cross)
		{
			homeUnloadResources();
			internet();
		}
		
		if (cursor->x >= 330 && cursor->x <= 374 && cursor->y >= 190 && cursor->y <= 240 && osl_keys->pressed.cross)
		{
			homeUnloadResources();
			openGmail();
		}
		
		if (cursor->x >= 100 && cursor->x <= 154 && cursor->y >= 195 && cursor->y <= 240 && osl_keys->pressed.cross)
		{
			homeUnloadResources();
			mp3player();
		}
		
		if (cursor->x >= 155 && cursor->x <= 210 && cursor->y >= 195 && cursor->y <= 240 && osl_keys->pressed.cross)
		{
			homeUnloadResources();
			messenger();
		}
			
		if (cursor->x >= 215 && cursor->x <= 243 && cursor->y >= 195 && cursor->y <= 230 && osl_keys->pressed.cross)
		{
			homeUnloadResources();
			appdrawer();
		}

		if (osl_keys->pressed.L)
		{	
			lockscreen();
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

