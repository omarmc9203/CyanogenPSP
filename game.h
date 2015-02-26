#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <psppower.h>
#include <string.h>
#include <stdio.h>
#include <oslib/oslib.h>

#define MAX_GAME_DISPLAY		6 // max amount of files displayed on-screen.
#define GAME_DISPLAY_X			10 // X value of where the filebrowser is displayed.
#define GAME_DISPLAY_Y			79 // Y value of the filebrowser is displayed.
#define GAME_CURR_DISPLAY_Y     74 

#define game_xSelection 0 //The distance between the X axis of the two selections
#define game_ySelection 29 //The distance between the Y axis of the two selections

OSL_IMAGE 	*gamebg, *gameSelection, *icon0;
OSL_FONT *Roboto;

SceCtrlData pad, oldpad;

void gameUp();
void gameDown();
void gameDisplay();
void gameControls();
char * gameBrowse(const char * path);
void gameUnload();
void gameView(char * browseDirectory);
int gameApp();
