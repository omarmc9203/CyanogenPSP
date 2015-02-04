#include "clock.h"
#include "lock.h"
#include "screenshot.h"

void lockscreen_deleteimages()
{
	oslDeleteImage(lock);
	oslDeleteImage(unlock);
	oslDeleteImage(messengericon1);
    oslDeleteImage(circle);
	oslDeleteImage(messengericon2);
	oslDeleteImage(music);
	oslDeleteImage(music2);	
	oslDeleteImage(circles);
}

int lockscreen()
{	
	int click = 0;
	int ending = 0;
	int style = 0;
	int musicApp = 0;
	int messengerApp = 0;
	
	setfont();
	
	lock = oslLoadImageFilePNG("System/Lockscreen/lock.png", OSL_IN_RAM, OSL_PF_8888);
	unlock = oslLoadImageFilePNG("System/Lockscreen/unlock.png", OSL_IN_RAM, OSL_PF_8888);
	circle = oslLoadImageFilePNG("System/Lockscreen/circle.png", OSL_IN_RAM, OSL_PF_8888);
	messengericon1 = oslLoadImageFilePNG("System/Lockscreen/messenger.png", OSL_IN_RAM, OSL_PF_8888);
	messengericon2 = oslLoadImageFilePNG("System/Lockscreen/messenger2.png", OSL_IN_RAM, OSL_PF_8888);
	music = oslLoadImageFilePNG("System/Lockscreen/music.png", OSL_IN_RAM, OSL_PF_8888);
	circles = oslLoadImageFilePNG("System/Lockscreen/circles.png", OSL_IN_RAM, OSL_PF_8888);
	music2 = oslLoadImageFilePNG("System/Lockscreen/music2.png", OSL_IN_RAM, OSL_PF_8888);

	if (!lock || !unlock || !circle || !messengericon1 || !music || !circles || !messengericon2 || !music2)
		debugDisplay();

	while (!osl_quit)
	{	
		LowMemExit();
	
		oslStartDrawing();
		
		controls();	

		oslDrawImage(background);	
		oslDrawImageXY(circles, 223, 240);
		
		battery(370,2,1);
		digitaltime(420,4,458);
		
		oslDrawImage(cursor);
		
		if (osl_pad.held.cross && cursor->x >= 190 && cursor->x <= 290 && cursor->y >= 120 && cursor->y <= 220)
		{
			click = 1;
		}
		
		if (! (osl_pad.held.cross))
			click = 0;
		if (ending == 1)
		if (style == 0 && ! (osl_pad.held.cross))
		{
		lockscreen_deleteimages();
		return;
		}
		
		if (click == 1)
		{
			oslDrawImageXY(messengericon1, 115,160);
			oslDrawImageXY(music, 216,76);
			oslDrawImageXY(circle, cursor->x - 50, cursor->y - 50);
		}
			
		if (ending == 1) 
		{
			oslDrawImageXY(unlock, 300,130);
		}
			
		else
		{
			oslDrawImageXY(lock, 300,130);	
		}
			
		if (cursor->x-50 >= 270 && cursor->x - 50 <= 330 && cursor->y - 50 >= 90 && cursor->y - 50 <= 160 && click == 1) 
		{
			ending = 1;
			oslDrawImageXY(unlock, 300,130);
		}
		else 
		{
			ending = 0;
			oslDrawImageXY(lock, 300,130);
		}
		
		if (cursor->x >= 216 && cursor->x <= 276 && cursor->y >= 66 && cursor->y <= 116 && click == 1) 
		{
			musicApp = 1;
			oslDrawImageXY(music2, 193,57);
		}
		
		if (cursor->x >= 90 && cursor->x <= 150 && cursor->y >= 125 && cursor->y <= 195 && click == 1) 
		{
			messengerApp = 1;
			oslDrawImageXY(messengericon2, 85,130);
		}
		
		if (musicApp == 1)
		{
			if (! (osl_pad.held.cross))
			{
				mp3player();
			}
		}
		
		if (messengerApp == 1)
		{
			if (! (osl_pad.held.cross))
			{
				messenger();
			}
		}
		
		if (click == 0)
		{
			oslDrawImageXY(circle, 190,120);
			oslDrawImageXY(lock, 190,120);
		}
		oslDrawImage(cursor);

		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
}

