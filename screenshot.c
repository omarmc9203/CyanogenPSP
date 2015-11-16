#include "fileManager.h"
#include "homeMenu.h"
#include "include/utils.h"
#include "screenshot.h"

unsigned int getbuttons();

int lastNumber = -1;

void captureScreenshot()
{
	unsigned int kernelButtons = getbuttons(); 
	
	if(kernelButtons & PSP_CTRL_NOTE)
	{ 
		oslPlaySound(KeypressStandard, 1);  
		screenshot();
	}
}

void genScreenshotFileName(int lastNumber, char *fileName, const char *ext) //Change the file name starting from 0001
{
	int a, b, c, d;

	a = lastNumber / 1000;
	lastNumber -= a * 1000;

	b = lastNumber / 100;
	lastNumber -= b * 100;

	c = lastNumber / 10;
	lastNumber -= c * 10;

	d = lastNumber;

	sprintf(fileName, "ms0:/PSP/GAME/CyanogenPSP/screenshots/screenshot%i%i%i%i%s", a, b, c, d, ext);
}

void screenshot() //Takes screenshot
{
	oslPlaySound(camera_click, 1);  
	sprintf(checkname, "%s", "screenshot"); 

	if(lastNumber == -1)
	{
		lastNumber = 0;
	}

	genScreenshotFileName(lastNumber, checkname, ".png"); 
	
	while (fileExists(checkname))
	{
		lastNumber++;
		genScreenshotFileName(lastNumber, checkname, ".png");
	}
	oslWriteImageFile(OSL_SECONDARY_BUFFER, checkname, 0);
	lastNumber++;
}

