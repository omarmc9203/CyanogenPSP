#include "homeMenu.h"
#include "clock.h"
#include "screenshot.h"
#include "powerMenu.h"
#include "lockScreen.h"
#include "recoveryMenu.h"
#include "settingsMenu.h"
#include "include/utils.h"
#include "prx/scepower.h"

void powermenu()
{	
	power = oslLoadImageFilePNG("system/home/menu/powerMenu.png", OSL_IN_RAM, OSL_PF_8888);
	powerSelection = oslLoadImageFilePNG("system/home/menu/powerSelection.png", OSL_IN_RAM, OSL_PF_8888);
	recoverySelection = oslLoadImageFilePNG("system/home/menu/recoverySelection.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!power || !powerSelection || !recoverySelection)
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
		navbarButtons(0);
		oslDrawImageXY(power, 100, 61);
		
		digitaltime(420,4,0,hrTime);
		battery(370,2,1);
		
		oslIntraFontSetStyle(Roboto, 0.75f, BLACK, 0, 0);
		oslDrawStringf(165, 100, "Power off");
		oslDrawStringf(165, 165, "Recovery");
		
		if (cursor->x >= 116 && cursor->x <= 364 && cursor->y >= 55 && cursor->y <= 125) 
		{
			oslDrawImageXY(powerSelection, 100, 60);
			oslDrawStringf(165, 100, "Power off");
			oslDrawStringf(165, 165, "Recovery");
			
			if (osl_keys->pressed.cross)
			{	
				oslPlaySound(KeypressStandard, 1);  
				sceKernelExitGame();
			}
			/*else if (osl_pad.held.cross)
			{
				scePowerRequestColdReset(0);
				scePowerRequestColdReset(50000);
			}*/
		}
		if (cursor->x >= 116 && cursor->x <= 364 && cursor->y >= 126 && cursor->y <= 200) 
		{
			oslDrawImageXY(recoverySelection, 100, 60);
			oslDrawStringf(165, 100, "Power off");
			oslDrawStringf(165, 165, "Recovery");
			if (osl_keys->pressed.cross)
			{	
				oslPlaySound(KeypressStandard, 1);  
				oslSyncFrame();
				sceKernelDelayThread(3*1000000);
				oslDeleteImage(power);
				oslDeleteImage(powerSelection);
				oslDeleteImage(recoverySelection);
				mainRecoveryMenu();
			}
		}
		
		oslDrawImage(cursor);
	
		if (osl_keys->pressed.circle)
		{	
			oslDeleteImage(power);
			oslDeleteImage(powerSelection);
			oslDeleteImage(recoverySelection);
			return;
		}
		
		captureScreenshot();
		
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();
	}
}
