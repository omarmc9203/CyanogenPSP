#include "calculator.h"
#include "fm.h"
#include "home.h"
#include "clock.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "screenshot.h"

OSL_COLOR black = RGB(0,0,0), red = RGB(255,0,0), white = RGB(255,255,255);

char x1[4][20] = {
	{"sin"},
	{"1/x"},
	{"."},
	{"."},
};

char x2[5][10] = {
	{"cos"},
	{"sqr"},
	{"."},
	{"."},
	{"C"},
};

char x3[4][10] = {
	{"tan"},
	{"^"},
	{""},
	{""},
};

int variable, selection;
 
int calculator()
{
	pgfFont = oslLoadFontFile("flash0:/font/ltn0.pgf");
    oslSetFont(pgfFont);

	int result;
	int x;
	int y;
	int enterednumber;
	int numberuse = 0;
	int suppression = 1;
	int Number = 0;

	calcbackground = oslLoadImageFile("system/app/calculator/calcbg.png", OSL_IN_RAM, OSL_PF_8888);

	if (!calcbackground)
		debugDisplay();
		
	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(calcbackground, 0, 19);
		
		x = 1;
		
		if (osl_keys->pressed.right)
		{
		x+=1;
		}
		
		if (x == 1)
		{
		
		for(variable=0;variable<4;variable++) {
			oslDrawStringf(40,variable*42+77,x1[variable]);
		}
		oslDrawStringf(40,selection*42+77,x1[selection]);
		
		if ((osl_keys->pressed.up) && (selection > 0))
			selection--;
		if ((osl_keys->pressed.down) && (selection < 3))
			selection++;
		}
		
		if (x == 2)
		{
		
		for(variable=0;variable<5;variable++) {
			oslDrawStringf(94,variable*42+77,x2[variable]);
		}
		oslDrawStringf(94,selection*42+77,x2[selection]);
		
		if ((osl_keys->pressed.up) && (selection > 0))
			selection--;
		if ((osl_keys->pressed.down) && (selection < 4))
			selection++;
		}
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.circle)
		{	
			oslDeleteImage(calcbackground);
			appdrawer();
		}
		
		if (osl_keys->pressed.L)
		{	
			lockscreen();
        }
		
		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			oslDeleteImage(calcbackground);
			appdrawer();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslDeleteImage(calcbackground);
			home();
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

