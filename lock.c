#include "clock.h"
#include "home.h"
#include "lock.h"
#include "screenshot.h"

int lockscreen()
{			
	lockscreenBg = oslLoadImageFilePNG("system/lockscreen/lockscreenBg.png", OSL_IN_RAM, OSL_PF_8888);

	if (!lockscreenBg)
		debugDisplay();

	while (!osl_quit)
	{	
		LowMemExit();
	
		oslStartDrawing();
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImage(background);	
		oslDrawImage(lockscreenBg);	
		
		battery(370,2,1);
		digitaltime(420,4,458);
		
		oslDrawImage(cursor);
		
		if (cursor->x >= 220 && cursor->x <= 260 && cursor->y >= 100 && cursor->y <= 272) 
		{
			if (osl_pad.held.cross && osl_keys->analogY <= -50)
			{
				oslDeleteImage(lockscreenBg);
				home();
			}
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

