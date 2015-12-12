#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <pspaudio.h>
#include <pspaudiolib.h>
#include <psppower.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <oslib/oslib.h>

#define MAX_MP3_DISPLAY			3 // max amount of files displayed on-screen.
#define MP3_DISPLAY_X			20 // X value of where the filebrowser is displayed.
#define MP3_DISPLAY_Y			108 // Y value of the filebrowser is displayed.
#define MP3_CURR_DISPLAY_Y     	98 
#define MP3DISPLAY_X        20

#define mp3XSelection 0 //The distance between the X axis of the two selections
#define mp3YSelection 55 //The distance between the Y axis of the two selections

OSL_IMAGE *mp3bg, *cursor, *nowplaying, *mp3_select, *mp3Play, *mp3Pause;

int MP3Scan(const char* path);
void mp3Up();
void mp3Down();
void mp3Upx5();
void mp3Downx5();
char *compact_str(char *s, int max_length);
void MP3Play(char * path);
int soundPlay(char * path);
void mp3Controls();
char * mp3Browse(const char * path);
int mp3View(char * browseDirectory);
int mp3player();