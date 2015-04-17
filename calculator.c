#include "calculator.h"
#include "include/utils.h"
#include "fm.h"
#include "home.h"
#include "clock.h"
#include "lock.h"
#include "multi.h"
#include "power_menu.h"
#include "screenshot.h"

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
	int result;
	int x;
	int y;
	int enterednumber;
	int numberuse = 0;
	int suppression = 1;
	int Number = 0;

	calcbackground = oslLoadImageFile("system/app/calculator/calcbg.png", OSL_IN_RAM, OSL_PF_8888);

	oslSetFont(Roboto);
	
	if (!calcbackground)
		debugDisplay();
		
	while (!osl_quit)
	{
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(calcbackground, 0, 0);
		
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
		
		digitaltime(420,4,0);
		battery(370,2,1);
		
		oslIntraFontSetStyle(Roboto, 0.8f,WHITE,0,0);

		oslDrawString(35,120,"7");
		oslDrawString(90,120,"8");
		oslDrawString(145,120,"9");
		oslDrawString(35,160,"4");
		oslDrawString(90,160,"5");
		oslDrawString(145,160,"6");
		oslDrawString(35,200,"1");
		oslDrawString(90,200,"2");
		oslDrawString(145,200,"3");
		oslDrawString(90,240,"0");

		oslDrawString(218,120,"/");
		oslDrawString(218,160,"x");
		oslDrawString(218,200,"-");
		oslDrawString(218,240,"+");
		oslIntraFontSetStyle(Roboto, 0.6f,WHITE,0,0);
		oslDrawString(260,120,"DEL");

		oslDrawString(326,120,"sin");
		oslDrawString(366,120,"cos");
		oslDrawString(406,120,"tan");
		oslDrawString(446,120,"ln");
	
		if (osl_keys->pressed.left)
		x = x++;
		if (x == 0)
		x = 8;

		if (osl_keys->pressed.right)
		x = x++;
		if (x == 10)
		x = 1;

		if (osl_keys->pressed.up)
		y = y--;
		if (y == 0)
		y = 4;

		if (osl_keys->pressed.down)
		y = y++;
		if (y == 5)
		y = 1;
		
		x = 1;
		y = 1;

			
			if (y == 1 && x ==1)
			{
				oslIntraFontSetStyle(Roboto, 0.8f,DARKGRAY,0,0);
				oslDrawString(35,120,"7");
			}
			else if (y == 1 && x == 2)
			{
				oslIntraFontSetStyle(Roboto, 0.8f,DARKGRAY,0,0);
				oslDrawString(90,120,"8");
			}
			else if (y == 1 && x  == 3) 
			{
				oslIntraFontSetStyle(Roboto, 0.8f,DARKGRAY,0,0);
				oslDrawString(145,120,"9");
			}
			else if (y == 1 && x == 4)
			{
				oslIntraFontSetStyle(Roboto, 0.8f,DARKGRAY,0,0);
				oslDrawString(218,120,"/");
			}
			else if (y == 1 && x == 5)
			{
				oslIntraFontSetStyle(Roboto, 0.6f,DARKGRAY,0,0);
				oslDrawString(260,120,"DEL");
			}
			else if (y == 1 && x  == 6) 
			{
				oslIntraFontSetStyle(Roboto, 0.6f,DARKGRAY,0,0);
				oslDrawString(326,120,"sin");
			}
			else if (y == 1 && x  == 7)
			{
				oslIntraFontSetStyle(Roboto, 0.6f,DARKGRAY,0,0);
				oslDrawString(366,120,"cos");
			}
			else if (y == 1 && x  == 8)
			{
				oslIntraFontSetStyle(Roboto, 0.6f,DARKGRAY,0,0);
				oslDrawString(406,120,"tan");
			}
			else if (y == 1 && x  == 9)
			{
				oslIntraFontSetStyle(Roboto, 0.6f,DARKGRAY,0,0);
				oslDrawString(446,120,"ln");
			}
			
			if (y == 2 && x == 1)
			{
				oslIntraFontSetStyle(Roboto, 0.8f,DARKGRAY,0,0);
				oslDrawString(35,160,"4");
			}
			else if (y == 2 && x == 2) 
			{
				oslIntraFontSetStyle(Roboto, 0.8f,DARKGRAY,0,0);
				oslDrawString(90,160,"5");
			}
			else if (y == 2 && x == 3)
			{
				oslIntraFontSetStyle(Roboto, 0.8f,DARKGRAY,0,0);
				oslDrawString(145,160,"6");
			}
			else if (y == 2 && x == 4) 
			{
				oslIntraFontSetStyle(Roboto, 0.8f,DARKGRAY,0,0);
				oslDrawString(218,160,"x");
			}

			if (y == 3 && x == 1)
			{
				oslIntraFontSetStyle(Roboto, 0.8f,DARKGRAY,0,0);
				oslDrawString(35,200,"1");
			}
			else if (y == 3 && x == 2)
			{
				oslIntraFontSetStyle(Roboto, 0.8f,DARKGRAY,0,0);
				oslDrawString(90,200,"2");
			}
			else if (y == 3 && x == 3)
			{
				oslIntraFontSetStyle(Roboto, 0.8f,DARKGRAY,0,0);
				oslDrawString(145,200,"3");
			}
			else if (y == 3 && x == 4)
			{
				oslIntraFontSetStyle(Roboto, 0.8f,DARKGRAY,0,0);
				oslDrawString(218,200,"-");
			}	

			if (y == 4 && x == 1)
			{
				oslIntraFontSetStyle(Roboto, 0.8f,DARKGRAY,0,0);
				oslDrawString(90,240,"0");
			}
			else if (y == 4 && x == 2)
			{
				oslIntraFontSetStyle(Roboto, 0.8f,DARKGRAY,0,0);
				oslDrawString(218,240,"+");
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
			oslPlaySound(Lock, 1);  
			lockscreen();
        }
		
		if (cursor->x >= 137 && cursor->x <= 200 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(calcbackground);
			appdrawer();
		}
		
		if (cursor->x >= 200 && cursor->x <= 276 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(calcbackground);
			home();
		}

		if (cursor->x >= 276 && cursor->x <= 340 && cursor->y >= 237 && cursor->y <= 271 && osl_keys->pressed.cross)
		{	
			oslPlaySound(KeypressStandard, 1);  
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

