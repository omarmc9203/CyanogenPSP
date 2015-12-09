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
#include <kubridge.h>
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
			
SceCtrlData pad, oldpad;

// Current Path
char cwd[1024];

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
	
	// Next Item
	struct File * next;
	// Folder Flag
	int isFolder;

} File;

File dirScan[MAX_FILES];

SceIoDirent g_dir;

int i;
int current;
int curScroll;
char lastDir[512];
int timer;
char returnMe[512];
/* Define a write buffer */
char write_buffer[128*1024];
char read_buffer[128*1024];

int copy_bytes(SceUID source, SceUID destination, unsigned bytes);
int copy_folder(char * source, char * destination);
void copy(int flag);
int paste(void);
int copy_file(char * a, char * b);
int copy_folder_recursive(char * a, char * b);
int fileExists(const char* path);
int dirExists(const char* path);
long getFileSize(const char * fileName);
int IsNextDir();
char* GetNextFileName();
int ChangeDir(const char* path);

int folderScan(const char* path);
int runFile(const char* path, char* type);
int openDir(const char* path, char* type);
void build_path(char *output, const char *root, const char *path, int append);
void write_file(const char *read_loc, const char *write_loc, const char *name);
void refresh();
void OptionMenu();
void renameFile();
void newFolder();
void deleteFile(char path[]);
int deleteRecursive(char path[]); //Thanks Moonchild!
void fcopy(char * source);
void fpaste(char * destination);
int checkTextFile(char *path);
char * readTextFromFile(char *path);
void displayTextFromFile(char * path);
void dirUpx5();
void dirDownx5();
void oslPrintText(int x, int y, float size, char * text, OSL_COLOR color);
void dirDisplay();
void centerText(int centerX, int centerY, char * centerText, int centerLength);
void dirVars();
void dirBack(int n);
void dirUp();
void dirDown();
void dirControls();
char * dirBrowse(const char * path);
void filemanager_unload();
int filemanage();