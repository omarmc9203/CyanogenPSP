#include "clock.h"
#include "appdrawer.h"
#include "home.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "screenshot.h"
#include "include/utils.h"

int hour = 0;
int minute = 0;
int second = 0;
int milisec = 0;

int counter = 0;
int timer = 0;

char shour[2];
char sminute[2];
char ssecond[2];
char smilisec[2];
char stimer[10];

void getDayOfWeek(int x, int y, int n)
{
	pspTime time;
    sceRtcGetCurrentClockLocalTime(&time);

	int day = sceRtcGetDayOfWeek(time.year, time.month, time.day);

    static const char days[7][16] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    
    char buffer[16];
	sprintf(buffer, "%s", days[day]);
    
    if(n == 2)
        buffer[3] = 0;
        
    oslDrawStringf(x,y,"%s", buffer);
}
	
void getMonthOfYear(int x, int y)
{
	static const char months[12][16] =
	{
		"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
	};
       
    pspTime time;
    sceRtcGetCurrentClockLocalTime(&time);
    oslDrawStringf(x,y,"%d %s", time.day, months[time.month - 1]);
}

/*Default x = 420  
  Default y = 4
  x is the location of the Time string on the X axis
  y is the location of the Time string on th Y axis
  colour is to indicate whether it should be in black/white
*/	

void digitaltime(int x, int y, int color) //color == 0 is white, color == 0 is black
{					
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);
	
	if (color == 0)
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
	else if (color == 1)
		oslIntraFontSetStyle(Roboto, 0.5f,BLACK,0,0);

	if(time.hour >= 12) 
		oslDrawString(x+38,y,"PM"); 
	else 
		oslDrawString(x+38,y,"AM");   
		
	if (time.hour > 12)
		time.hour -= 12;
	
	if (time.hour == 00)
	time.hour = 12;
	
	oslDrawStringf(x,y,"%2d:%02d", time.hour, time.minutes);	
}

void centerClock(int n) 
{					
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);
	
	if (time.hour > 12)
		time.hour -= 12;
	
	if (time.hour == 00)
		time.hour = 12;
		
    oslIntraFontSetStyle(Roboto, 1.7f,WHITE,0,INTRAFONT_ALIGN_CENTER);
	
	if (n==0)
	{
		oslDrawStringf(222,136,"%2d:%02d", time.hour, time.minutes);
	}
	
	else if (n == 1)
	{
		oslDrawStringf(235,70,"%2d:%02d", time.hour, time.minutes);	
	}
}

void startCounter()
{
	SceCtrlData newPad, oldPad;

	while (!osl_quit)
	{
		oslStartDrawing();
		
		oldPad = newPad;
		sceCtrlReadBufferPositive(&newPad, 1);
		if (newPad.Buttons != oldPad.Buttons)
		{
			if (newPad.Buttons & PSP_CTRL_CROSS)
			{
				oslPlaySound(KeypressStandard, 1);  
				break;
			}
		}

		if (hour == 60)
		{
			hour = 0;
		}
		if (minute == 60)
		{
			hour++;
			minute = 0;
		}
		if (second == 60)
		{
			minute++;
			second = 0;
		}
		if (milisec == 60)
		{
			second++;
			milisec = milisec *1.67;
		}
		oslDrawStringf(200,200,"%d:%d:%d.%d       ", hour, minute, second, milisec);
		milisec++;
	}
	oslEndDrawing(); 
	oslEndFrame(); 
	oslSyncFrame();
}

void stopWatch()
{
	stop_watch = oslLoadImageFilePNG("system/app/clock/stop_watch.png", OSL_IN_RAM, OSL_PF_8888);

	if (!stop_watch)
		debugDisplay();

	oslSetFont(Roboto);
	
	SceCtrlData newPad, oldPad;
	sceCtrlReadBufferPositive(&oldPad, 1);
	
	while (!osl_quit)
	{	
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslDrawImageXY(stop_watch, 0, 19);

		digitaltime(381,4,0);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
			oslDeleteImage(stop_watch);
			appdrawer();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(stop_watch);
			appdrawer();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(stop_watch);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		if (cursor->x >= 114 && cursor->x <= 150  && cursor->y >= 19 && cursor->y <= 50 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(stop_watch);
			pspclock();
		}
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
		
		oldPad = newPad;
		sceCtrlReadBufferPositive(&newPad, 1);

		if (newPad.Buttons != oldPad.Buttons)
		{	
			if (newPad.Buttons & PSP_CTRL_CROSS)
			{
				oslPlaySound(KeypressStandard, 1);  
				if (counter == 0)
				{
					counter = 1;
					startCounter();
				}
			}
			if (newPad.Buttons & PSP_CTRL_RTRIGGER)
			{	
			hour = 0;
			minute = 0;
			second = 0;
			milisec = 0;
			oslDrawStringf(211,175, "0:0:0.00       ");
			counter = 0;
			}
		}
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();
	}
}

int pspclock()
{
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);

	clockBg = oslLoadImageFilePNG("system/app/clock/clockBg.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (time.hour >= 0.00  && time.hour <= 11.59)
	{
		timeBg = oslLoadImageFilePNG("system/app/clock/timeBg5.png", OSL_IN_RAM, OSL_PF_8888);
	}
	else if (time.hour >= 12.00  && time.hour <= 14.59)
	{
		timeBg = oslLoadImageFilePNG("system/app/clock/timeBg1.png", OSL_IN_RAM, OSL_PF_8888);
	}
	else if (time.hour >= 15.00 && time.hour <= 17.59)
	{
		timeBg = oslLoadImageFilePNG("system/app/clock/timeBg2.png", OSL_IN_RAM, OSL_PF_8888);
	}
	else if (time.hour >= 18.00  && time.hour <= 20.59)
	{
		timeBg = oslLoadImageFilePNG("system/app/clock/timeBg3.png", OSL_IN_RAM, OSL_PF_8888);
	}
	else if (time.hour >= 21.00  && time.hour <= 23.59)
	{
		timeBg = oslLoadImageFilePNG("system/app/clock/timeBg4.png", OSL_IN_RAM, OSL_PF_8888);
	}
	
	if (!clockBg || !timeBg)
		debugDisplay();

	oslSetFont(Roboto);
	
	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslDrawImageXY(timeBg, 0, 0);
		oslDrawImageXY(clockBg, 0, 0);
		
		if (time.hour > 12)
			time.hour -= 12;
	
		if (time.hour == 00)
			time.hour = 12;
		
        centerClock(0);
		
		oslIntraFontSetStyle(Roboto, 0.6f,WHITE,BLACK,INTRAFONT_ALIGN_CENTER);
		if(time.hour <= 12) 
		oslDrawString(300,136,"PM"); 
		else if (time.hour >= 12) 
		oslDrawString(300,136,"AM");  
		
		getDayOfWeek(190,156,2);
		getMonthOfYear(265,156);
		
        oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);

		digitaltime(381,4,0);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
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
			oslDeleteImage(clockBg);
			oslDeleteImage(timeBg);
			appdrawer();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
	
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(clockBg);
			oslDeleteImage(timeBg);
			appdrawer();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(clockBg);
			oslDeleteImage(timeBg);
			home();
		}
		
		/*
		if (cursor->x >= 285 && cursor->x <= 332  && cursor->y >= 19 && cursor->y <= 50 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(clockBg);
			oslDeleteImage(timeBg);
			stopWatch();
		}
		*/
		
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
	return 0;
}