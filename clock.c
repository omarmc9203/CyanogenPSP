#include "clock.h"
#include "appDrawer.h"
#include "homeMenu.h"
#include "lockScreen.h"
#include "recentsMenu.h"
#include "powerMenu.h"
#include "screenshot.h"
#include "settingsMenu.h"
#include "include/utils.h"

time_t		currentTime;
time_t		startTimer_;

int timeState;
int timerStart = 0;

typedef struct
{
	clock_t measuredTime;
    clock_t offset;
} Timer;

void startTimer() 
{
	//Get current time
	sceKernelLibcTime(&startTimer_);
	
	// Set Timer initiation to 1
	timerStart = 1;
}

void pauseTimer() 
{
	timeState = timeState;
}

int getTimer()
{
	if(!timerStart)
		return 0;

	//Get the current time
	sceKernelLibcTime(&currentTime);

	//Return the difference between current time and the start time.
	timeState = ((int)currentTime - (int)startTimer_);
	
	return timeState;
}

void resetTimer() 
{
	timerStart = 0; //Reset to 0
}

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

void digitaltime(int x, int y, int color, int hr) //color == 0 is white, color == 0 is black
{					
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);
	
	if (color == 0)
		oslIntraFontSetStyle(Roboto, 0.5, WHITE, 0, 0);
	else if (color == 1)
		oslIntraFontSetStyle(Roboto, 0.5, BLACK, 0, 0);
		
	if (hr == 0)
	{
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
	else if (hr == 1)
	{
		oslDrawStringf(x+15,y,"%2d:%02d", time.hour, time.minutes);
	}
}

void centerClock(int n) 
{					
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);
	
	if (hrTime == 0)
	{
		if (time.hour > 12)
			time.hour -= 12;
	
		if (time.hour == 00)
			time.hour = 12;
	}
	
    oslIntraFontSetStyle(Roboto, 1.7f, WHITE, 0, INTRAFONT_ALIGN_CENTER);
	
	if (n==0)
	{
		oslDrawStringf(222,136,"%2d:%02d", time.hour, time.minutes);
	}
	
	else if (n == 1)
	{
		oslDrawStringf(235,70,"%2d:%02d", time.hour, time.minutes);	
	}
}

int pspTimer()
{
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);

	timerBg = oslLoadImageFilePNG("system/app/clock/timerBg.png", OSL_IN_RAM, OSL_PF_8888);
	timerPause = oslLoadImageFilePNG("system/app/clock/pause.png", OSL_IN_RAM, OSL_PF_8888);
	timerPlay = oslLoadImageFilePNG("system/app/clock/play.png", OSL_IN_RAM, OSL_PF_8888);
	
	loadBg();
	
	if (!timerBg || !timeBg || !timerPause || !timerPlay)
		debugDisplay();

	oslSetFont(Roboto);
	
	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslDrawImageXY(timeBg, 0, 0);
		oslDrawImageXY(timerBg, 0, 0);
		
		if (timerStart == 1)
			oslDrawImageXY(timerPause, 200, 204);
		else 
			oslDrawImageXY(timerPlay, 200, 204);
		
		if (time.hour > 12)
			time.hour -= 12;
	
		if (time.hour == 00)
			time.hour = 12;
		
		oslIntraFontSetStyle(Roboto, 0.6f, WHITE, BLACK, INTRAFONT_ALIGN_CENTER);
		
		oslDrawStringf(224,110,"%d\n", getTimer());

        oslIntraFontSetStyle(Roboto, fontSize, WHITE, 0, 0);

		digitaltime(381,4,0,hrTime);
		battery(330,2,0);
		navbarButtons(2);
		androidQuickSettings();
		volumeController();
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
			oslDeleteImage(timerBg);
			oslDeleteImage(timeBg);
			appdrawer();
		}
		
		if (cursor->x >= 108 && cursor->x <= 140  && cursor->y >= 25 && cursor->y <= 54 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(timerBg);
			oslDeleteImage(timeBg);
			pspclock();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
	
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(timerBg);
			oslDeleteImage(timeBg);
			appdrawer();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(timerBg);
			oslDeleteImage(timeBg);
			home();
		}

		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
	return 0;
}

int pspStopWatch()
{
	int hour = 0, min = 0, miliSec = 0;
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);

	stopWatchBg = oslLoadImageFilePNG("system/app/clock/stopWatchBg.png", OSL_IN_RAM, OSL_PF_8888);
	timerPause = oslLoadImageFilePNG("system/app/clock/pause.png", OSL_IN_RAM, OSL_PF_8888);
	timerPlay = oslLoadImageFilePNG("system/app/clock/play.png", OSL_IN_RAM, OSL_PF_8888);
	reset = oslLoadImageFilePNG("system/app/clock/reset.png", OSL_IN_RAM, OSL_PF_8888);
	
	loadBg();
	
	if (!stopWatchBg || !timeBg || !timerPause || !timerPlay)
		debugDisplay();

	oslSetFont(Roboto);
	
	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	
		
		oslDrawImageXY(timeBg, 0, 0);
		oslDrawImageXY(stopWatchBg, 0, 0);
		
		if (timerStart == 1)
		{
			//oslDrawImageXY(timerPause, 200, 215);
			oslDrawImageXY(reset, 80, 222);
		}
		else 
			oslDrawImageXY(timerPlay, 200, 215);
			
		oslIntraFontSetStyle(Roboto, 0.7f, WHITE, BLACK, INTRAFONT_ALIGN_CENTER);
		
		oslDrawStringf(224,110,"%d %d %d %d\n", hour, min, getTimer(), miliSec);

		if (timerStart == 1)
		{
			miliSec+=1;
			if (miliSec > 99)
			miliSec = 0;
		}
		
		if (timeState >= 60)
		{
			min += 1;
			resetTimer();
			startTimer();
		}
		
		if (min >= 60)
		{
			hour += 1;
			min = 0;
		}

        oslIntraFontSetStyle(Roboto, fontSize, WHITE, 0, 0);

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (cursor->x >= 196 && cursor->x <= 246  && cursor->y >= 208 && cursor->y <= 258 && timerStart == 0 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			startTimer();
		}
		
		else if (cursor->x >= 196 && cursor->x <= 246  && cursor->y >= 208 && cursor->y <= 258 && timerStart == 1 && osl_keys->pressed.cross)
		{
			pauseTimer();
			oslPlaySound(KeypressStandard, 1);  
		}
		
		if (cursor->x >= 70 && cursor->x <= 106  && cursor->y >= 216 && cursor->y <= 248 && osl_keys->pressed.cross)
		{
			miliSec = 0;
			min = 0;
			hour = 0;
			oslPlaySound(KeypressStandard, 1);  
			resetTimer();
		}
		
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
			oslDeleteImage(stopWatchBg);
			oslDeleteImage(timeBg);
			oslDeleteImage(reset);
			oslDeleteImage(timerPlay);
			oslDeleteImage(timerPause);
			appdrawer();
		}
		
		if (cursor->x >= 108 && cursor->x <= 140  && cursor->y >= 25 && cursor->y <= 54 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(stopWatchBg);
			oslDeleteImage(timeBg);
			oslDeleteImage(reset);
			oslDeleteImage(timerPlay);
			oslDeleteImage(timerPause);
			pspclock();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
	
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(stopWatchBg);
			oslDeleteImage(timeBg);
			oslDeleteImage(reset);
			oslDeleteImage(timerPlay);
			oslDeleteImage(timerPause);
			appdrawer();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(stopWatchBg);
			oslDeleteImage(timeBg);
			oslDeleteImage(reset);
			oslDeleteImage(timerPlay);
			oslDeleteImage(timerPause);
			home();
		}

		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
	return 0;
}

void loadBg()
{
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);

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
}

int pspclock()
{
	pspTime time;
	sceRtcGetCurrentClockLocalTime(&time);

	clockBg = oslLoadImageFilePNG("system/app/clock/clockBg.png", OSL_IN_RAM, OSL_PF_8888);
	
	loadBg();
	
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
		
        centerClock(0);
		
		oslIntraFontSetStyle(Roboto, 0.6f, WHITE, BLACK, INTRAFONT_ALIGN_CENTER);
		
		if (hrTime == 0)
		{			
			if(time.hour <= 12) 
				oslDrawString(300,136,"PM"); 
			else if (time.hour >= 12) 
				oslDrawString(300,136,"AM");  
		}
		
		getDayOfWeek(190,156,2);
		getMonthOfYear(265,156);

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
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
		
		/* PSP timer
		if (cursor->x >= 186 && cursor->x <= 214  && cursor->y >= 25 && cursor->y <= 54 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(clockBg);
			oslDeleteImage(timeBg);
			pspTimer();
		}
		*/
		
		if (cursor->x >= 258 && cursor->x <= 286  && cursor->y >= 25 && cursor->y <= 54 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(clockBg);
			oslDeleteImage(timeBg);
			pspStopWatch();
		}
		
		captureScreenshot();
		
	oslEndDrawing(); 
    oslEndFrame(); 
	oslSyncFrame();
	}
	return 0;
}