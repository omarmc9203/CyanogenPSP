#include "screenshot.h"

void makeScreenshotDir()
{
	SceUID dir = sceIoDopen(screenshotpath);
	
	if (dirExists(screenshotpath))
	{
		sceIoDclose(dir);
	}
	else 
	{
		sceIoMkdir("ms0:/PSP/GAME/CyanogenPSP/screenshots",0777);
	}
}

static void genScreenshotFileName(int lastNumber, char *fileName, const char *ext) //Change the file name starting from 0001
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
	makeScreenshotDir();

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

