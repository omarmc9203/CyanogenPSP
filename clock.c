#include <pspkernel.h>
#include <pspdebug.h>
#include <oslib/oslib.h>
#include <pspctrl.h>
#include <psprtc.h>
#include "clock.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "screenshot.h"

OSL_IMAGE *clockbg, *cursor, *stop_watch,  *backicon, *homeicon, *multicon;
OSL_FONT *clockFont;

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

void centerclock()
{
	clockFont = oslLoadIntraFontFile("system/fonts/DroidSans.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
	oslIntraFontSetStyle(clockFont, 0.9f, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	oslSetFont(clockFont);
	
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);
	
	if (time.hour > 12)
		time.hour -= 12;
	
	oslDrawStringf(240,136,"%02d:%02d", time.hour, time.minutes);
	
}

void getDayOfWeek(int x, int y) //Outputs the Day of the Week
{
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);
	
	if (sceRtcGetDayOfWeek(time.year, time.month, time.day)==1)
	{
		oslDrawStringf(x,y,"MONDAY");
	}
	else if (sceRtcGetDayOfWeek(time.year, time.month, time.day)==2)
	{
		oslDrawStringf(x,y,"TUESDAY");
	}
	else if (sceRtcGetDayOfWeek(time.year, time.month, time.day)==3)
	{
		oslDrawStringf(x,y,"WEDNESDAY");
	}
	else if (sceRtcGetDayOfWeek(time.year, time.month, time.day)==4)
	{
		oslDrawStringf(x,y,"THURSDAY");
	}
	else if (sceRtcGetDayOfWeek(time.year, time.month, time.day)==5)
	{
		oslDrawStringf(x,y,"FRIDAY");
	}
	else if (sceRtcGetDayOfWeek(time.year, time.month, time.day)==6)
	{
		oslDrawStringf(x,y,"SATURDAY");
	}
	else
	{
		oslDrawStringf(x,y,"SUNDAY");
	}
}

void getMonthOfYear(int x, int y) //Outputs the Month of the Year
{ 
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);

	if (time.month == 1)
	{
		oslDrawStringf(x,y,"%d JANUARY", time.day);
	}
	else if (time.month == 2)
	{
		oslDrawStringf(x,y,"%d FEBRUARY", time.day);
	}
	else if (time.month == 3)
	{
		oslDrawStringf(x,y,"%d MARCH", time.day);
	}
	else if (time.month == 4)
	{
		oslDrawStringf(x,y,"%d APRIL", time.day);
	}
	else if (time.month == 5)
	{
		oslDrawStringf(x,y,"%d MAY", time.day);
	}
	else if (time.month == 6)
	{
		oslDrawStringf(x,y,"%d JUNE", time.day);
	}
	else if (time.month == 7)
	{
		oslDrawStringf(x,y,"%d JULY", time.day);
	}
	else if (time.month == 8)
	{
		oslDrawStringf(x,y,"%d AUGUST", time.day);
	}
	else if (time.month == 9)
	{
		oslDrawStringf(x,y,"%d SEPTEMBER", time.day);
	}
	else if (time.month == 10)
	{
		oslDrawStringf(x,y,"%d OCTOBER", time.day);
	}
	else if (time.month == 11)
	{
		oslDrawStringf(x,y,"%d NOVEMBER", time.day);
	}
	else
	oslDrawStringf(x,y,"%d DECEMBER", time.day);
}

/*Default x = 420, x2 = 458  
  Default y = 4
  x is the location of the Time string on the X axis
  y is the location of the Time string on th Y axis
  x2 is the location of the characters "PM/AM" on the X axis
*/	

void digitaltime(int x, int y, int x2) 
{					
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);

	if(time.hour >= 12) 
		oslDrawString(x2,y,"PM"); 
	else 
		oslDrawString(x2,y,"AM");   
		
	if (time.hour > 12)
		time.hour -= 12;
	
	if (time.hour == 00)
	time.hour = 12;
	
	oslDrawStringf(x,y,"%2d:%02d", time.hour, time.minutes);	

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
	
	setfont();
	
	SceCtrlData newPad, oldPad;
	sceCtrlReadBufferPositive(&oldPad, 1);
	
	while (!osl_quit)
	{	
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslDrawImageXY(stop_watch, 0, 19);

		battery(337,2,0);
		navbarButtons(1);
		androidQuickSettings();
		digitaltime(420,4,458);
		oslDrawImage(cursor);

		if (osl_keys->pressed.square)
		{	
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(stop_watch);
			appdrawer();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			oslDeleteImage(stop_watch);
			appdrawer();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(stop_watch);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			multitask();
		}
		
		if (cursor->x >= 114 && cursor->x <= 150  && cursor->y >= 19 && cursor->y <= 50 && osl_keys->pressed.cross)
		{
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
	clockbg = oslLoadImageFilePNG("system/app/clock/clockbg.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!clockbg)
		debugDisplay();
	
	setfont();
	
	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslDrawImageXY(clockbg, 0, 19);

		battery(337,2,0);
		navbarButtons(1);
		androidQuickSettings();
		digitaltime(420,4,458);
		oslDrawImage(cursor);

		if (osl_keys->pressed.square)
		{	
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			lockscreen();
        }
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(clockbg);
			appdrawer();
		}

		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			oslDeleteImage(clockbg);
			appdrawer();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(clockbg);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			multitask();
		}
		
		if (cursor->x >= 285 && cursor->x <= 332  && cursor->y >= 19 && cursor->y <= 50 && osl_keys->pressed.cross)
		{
			oslDeleteImage(clockbg);
			stopWatch();
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



