#include <oslib/oslib.h>

#define screenshotpath "ms0:/PSP/GAME/CyanogenMod/screenshots"

char checkname[256];

void captureScreenshot();
void makeScreenshotDir();
void genScreenshotFileName(int lastNumber, char *fileName, const char *ext);
void screenshot();