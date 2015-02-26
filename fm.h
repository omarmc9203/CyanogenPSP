#include <pspsdk.h>
#include <pspkernel.h>
#include <string.h>
#include <stdio.h>
#include <systemctrl.h>
#include <psppower.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspsdk.h>
#include <unistd.h>
#include <pspiofilemgr.h>
#include <pspinit.h>
#include <stdlib.h>
#include <oslib/oslib.h>
#include <psploadexec.h>
#include <psploadexec_kernel.h>

#define rootdir "ms0:/"

#define MAX_FILES			256 // max amount of files needed to load.
#define MAX_DISPLAY			5 // max amount of files displayed on-screen.
#define DISPLAY_X			85 // X value of where the filebrowser is displayed.
#define DISPLAY_Y			70 // Y value of the filebrowser is displayed.
#define ICON_DISPLAY_Y      56 
#define CURR_DISPLAY_Y     	52
#define MP3DISPLAY_X        20

// Copy Flags
#define COPY_FOLDER_RECURSIVE 2
#define COPY_DELETE_ON_FINISH 1
#define COPY_KEEP_ON_FINISH 0
#define NOTHING_TO_COPY -1

// Default Start Path
#define START_PATH "ms0:/PSP/SAVEDATA/"

OSL_IMAGE 	*filemanagerbg, *diricon, *imageicon, *mp3icon, *txticon, *unknownicon, *documenticon, *binaryicon, *videoicon, *archiveicon, *bar, 
			*deletion, *action, *textview,  *gallerybar;

OSL_FONT *Roboto;

// Current Path
static char cwd[1024];

/* Globals */

typedef struct fileIcon {

	int		active;

	char	name[256];
	char	filePath[256];
	char	fileType[256];

	int		x;
	int		y;

} fileIcon;

fileIcon folderIcons[MAX_FILES];

typedef struct File {

	int exist;

	char path[256];
	char name[256];

	int size;
	int directory;

} File;

File dirScan[MAX_FILES];

SceIoDirent g_dir;

char *file;
int i;
int current;
int curScroll;
char lastDir[512];
int timer;
char returnMe[512];
SceCtrlData pad, oldpad;

/*
// Copy Move Origin
static char copysource[1024];
int copymode = NOTHING_TO_COPY

// File List
File * files = NULL;
*/

int folderScan(const char* path);
int runFile(const char* path, char* type );
int openDir(const char* path, char* type);
void build_path(char *output, const char *root, const char *path, int append);
void write_file(const char *read_loc, const char *write_loc, const char *name);
void refresh();
void OptionMenu();
void DeleteFile(const char * path);
void recursiveDelete(const char *dir);
void fcopy(char * source);
void fpaste(char * destination);
int checkTextFile(char *textfile);
char *getTextFromFile();
void displayTextFromFile();
void dirUpx5();
void dirDownx5();
void oslPrintText(int x, int y, float size, char * text, OSL_COLOR color);
void dirDisplay();
void centerText(int centerX, int centerY, char * centerText, int centerLength);
void dirVars();
void dirBack();
void dirUp();
void dirDown();
int launchEbootMs0(const char *path[]);
int launchEbootEf0(const char *path[]);
void dirControls();
char * dirBrowse(const char * path);
void filemanager_unload();
int filemanage();