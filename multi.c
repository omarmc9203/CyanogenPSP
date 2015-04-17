#include "clock.h"
#include "home.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "screenshot.h"
#include "include/utils.h"

int multitask()
{	
	multi_task = oslLoadImageFilePNG("system/home/menu/multi_task.png", OSL_IN_RAM, OSL_PF_8888);

	if (!multi_task)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		controls();
		
		oslDrawImageXY(background, 0,0);
		oslDrawImageXY(multi_task, 0,10);
		
		oslDrawString(130,136,"Your recent screens appear here");

		digitaltime(420,4,0);
		battery(370,2,1);
		navbarButtons(0);
		androidQuickSettings();
		oslDrawImage(cursor);
		
		if (osl_pad.held.square)
		{
			powermenu();
		}
		
		if (osl_pad.held.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (osl_pad.held.circle)
		{	
			oslDeleteImage(multi_task);
			return;
		}
		
		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(multi_task);
			return;
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(multi_task);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_pad.held.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(multi_task);
			multitask();
		}
		
		if (osl_pad.held.R && osl_pad.held.triangle)
		{
			screenshot();
		}		
	
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
}