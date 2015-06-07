#include <pspkernel.h>
#include <pspctrl.h>
#include <pspumd.h>

//PSP Net Stuff
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspwlan.h>

//OSLib
#include <oslib/oslib.h>

//File Management
#include <pspiofilemgr.h>
#include <pspiofilemgr_kernel.h>
#include <pspiofilemgr_dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h> 
#include <stdlib.h> 

#define configFile "system/build.prop"
#define Address "www.google.com"
#define MAX 8

#define stateOff 0
#define stateNoClock 1
#define stateOverClock 2
#define stateUnderClock 3

#define MAX_SETTINGS_DISPLAY		4 // max amount of files displayed on-screen.
#define SETTINGS_DISPLAY_X			20 // X value of where the filebrowser is displayed.
#define SETTINGS_DISPLAY_Y			82 // Y value of the filebrowser is displayed.
#define SETTINGS_CURR_DISPLAY_Y     62 

//declaration
OSL_IMAGE *settingsbg, *cursor, *usbdebug, *aboutbg, *offswitch, *onswitch, *displaybg, *performancebg, *wifibg, *developerbg, *about, *highlight, 
		  *developeroptions, *themes, *wifi, *processorbg, *cpuset, *check, *updatesbg, *performance, *recoverybg, *security, *securitybg, *music;

//definition of our sounds
OSL_SOUND *tone;

struct fw_version
{
	unsigned int major;
	unsigned int minor;
};

typedef struct fw_version fw_version;

SceUID modules[3];

int connectAPCallback(int state);
int connectToAP(int config);
char fontPath[100];
char backgroundPath[100], nickname[25], pspname[25];
int widgetActivator, eDesktopActivator, bootAnimActivator, gBootActivator, processorState, hrTime, language, usbStat, isPlaying;
int passProtect;
int RJL;
int PSPDebug;
void onlineUpdater();
void switchStatus(int n);
void loadConfig();
void saveConfig();
int getCpuClock();
int getBusClock();
void pspGetModel(int x, int y);
void aboutMenu();
void easterEgg();
void updatesMenu();
void performanceMenu();
void setCpuBoot();
void processorMenu();
void ramMenu();
void storageMenu();
void displayMenu();
void displayThemes();
void settingsDisplay();
void settingsUp();
void settingsDown();
void settingsUpx5();
void settingsDownx5();
void changeFont();
void settingsControls(int n);
char * settingsBrowse(const char * path, int n);
void displaySubThemes(char * browseDirectory, int n);
void displayTime();
void displayMiscellaneous();
void securityMenu();
void wifiMenu();
void DumpBootBin(void);
void dump_filesystem(const char *root, const char *write_loc);
void dump_memory(void);
void dumpMenu();
void dumpMenuMore();
void developerMenu();
void settingsHighlight();
void settingsDeleteResources();
void settingsMenu();