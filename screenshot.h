#include <oslib/oslib.h>

#define screenshotpath "ms0:/PSP/GAME/CyanogenMod/screenshots"

char checkname[256];
static int lastNumber = -1;

void makeScreenshotDir();
static void genScreenshotFileName(int lastNumber, char *fileName, const char *ext);
void screenshot();