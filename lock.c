#include "clock.h"
#include "home.h"
#include "lock.h"
#include "screenshot.h"
#include "settingsmenu.h"
#include "include/utils.h"

int lockscreen()
{	
	FILE * password;
	u8 Keyboard = 0;
	char passwordData[20] = "";
	char data[20] = "";
	
	lockscreenBg = oslLoadImageFilePNG("system/lockscreen/lockscreenBg.png", OSL_IN_RAM, OSL_PF_8888);

	if (!lockscreenBg)
		debugDisplay();
		
	if (fileExists("system/settings/password.txt"))
		passProtect = 1;

	while (!osl_quit)
	{	
		LowMemExit();
	
		oslStartDrawing();
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImage(background);	
		oslDrawImage(lockscreenBg);
		
		centerClock(1);
		
		oslIntraFontSetStyle(Roboto, 0.4f,WHITE,0,0);
		getDayOfWeek(180,90,1);
		getMonthOfYear(250,90);
		oslDrawStringf(20,20,"%d",passProtect); 

		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
		
		battery(370,2,1);
		digitaltime(420,4,458);
		androidQuickSettings();
		
		oslDrawImage(cursor);
		
		if (passProtect == 1)
		{
			if (cursor->x >= 220 && cursor->x <= 260 && cursor->y >= 100 && cursor->y <= 272) 
			{
				if (osl_pad.held.cross && osl_keys->analogY <= -50)
				{
					if (Keyboard == 0)
					{
						oslInitOsk("Enter Password", "", 20, 1, 1); 
						Keyboard = 1; 
					}
					if (Keyboard == 1)
					{
						oslDrawOsk(); 
						if (oslGetOskStatus() == PSP_UTILITY_DIALOG_NONE)
						{
							if (oslOskGetResult() == OSL_OSK_CANCEL)
							{		 
								Keyboard = -1; 
							}
							else
							{
								oslOskGetText(data); 
								Keyboard = 2; 
							}
							oslEndOsk(); 
						}
					}
					password = fopen("system/settings/password.txt", "r");
					fscanf(password,"%s",passwordData);
					fclose(password);
					
					if (strcmp(passwordData,data == 0))
					{
						Keyboard = 0;
						oslDeleteImage(lockscreenBg);
						home();
					}
					else 
					{
						oslDrawStringf(140,136,"Password Incorrect, try again");
						Keyboard = 0;
					}
				}
			}
		}
		
		else if (passProtect == 0)
		{
			if (cursor->x >= 220 && cursor->x <= 260 && cursor->y >= 100 && cursor->y <= 272) 
			{
				if (osl_pad.held.cross && osl_keys->analogY <= -50)
				{
					oslDeleteImage(lockscreenBg);
					home();
				}
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

