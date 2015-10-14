#include "homeMenu.h"
#include "clock.h"
#include "screenshot.h"
#include "powerMenu.h"
#include "lockScreen.h"
#include "recoveryMenu.h"
#include "settingsMenu.h"

void powermenu()
{	
	
	power = oslLoadImageFilePNG("system/home/menu/power.png", OSL_IN_RAM, OSL_PF_8888);
	power1 = oslLoadImageFilePNG("system/home/menu/power1.png", OSL_IN_RAM, OSL_PF_8888);
	recovery1 = oslLoadImageFilePNG("system/home/menu/recovery1.png", OSL_IN_RAM, OSL_PF_8888);
	screenshot1 = oslLoadImageFilePNG("system/home/menu/screenshot1.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!power || !power1 || !recovery1 || !screenshot1)
		debugDisplay();

	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();

		controls();	

		oslClearScreen(RGB(0,0,0));	
		oslDrawImage(background);
		oslDrawImageXY(ic_launcher_apollo, 105, 190);
		oslDrawImageXY(ic_launcher_browser, 276, 190);
		oslDrawImageXY(ic_launcher_settings, 331, 190);
		oslDrawImageXY(ic_launcher_messenger, 160, 190);
		oslDrawImageXY(pointer, 231, 180);
		navbarButtons(0);
		oslDrawImageXY(power, 102, 41);		
		
		digitaltime(420,4,0,hrTime);
		battery(370,2,1);
		
		if (cursor->x >= 116 && cursor->x <= 364 && cursor->y >= 47 && cursor->y <= 106) 
		{
			oslDrawImageXY(power1, 102, 47);
			if (osl_keys->pressed.cross)
			{	
				oslPlaySound(KeypressStandard, 1);  
				sceKernelExitGame();
			}
		}
		if (cursor->x >= 116 && cursor->x <= 364 && cursor->y >= 106 && cursor->y <= 165) 
		{
			oslDrawImageXY(recovery1, 102, 106);
			if (osl_keys->pressed.cross)
			{	
				oslPlaySound(KeypressStandard, 1);  
				oslSyncFrame();
				sceKernelDelayThread(3*1000000);
				oslDeleteImage(power);
				oslDeleteImage(power1);
				oslDeleteImage(recovery1);
				oslDeleteImage(screenshot1);
				mainRecoveryMenu();
			}
		}
		if (cursor->x >= 116 && cursor->x <= 364 && cursor->y >= 165 && cursor->y <= 224) 
		{
			oslDrawImageXY(screenshot1, 102, 165);
			if (osl_keys->pressed.cross)
			{	
				oslPlaySound(KeypressStandard, 1);  
				oslDeleteImage(power);
				oslDeleteImage(power1);
				oslDeleteImage(recovery1);
				oslDeleteImage(screenshot1);
				return;
				screenshot();
			}
		}
		
		oslDrawImage(cursor);
	
		if (osl_keys->pressed.circle)
		{	
			oslDeleteImage(power);
			oslDeleteImage(power1);
			oslDeleteImage(recovery1);
			oslDeleteImage(screenshot1);
			return;
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
