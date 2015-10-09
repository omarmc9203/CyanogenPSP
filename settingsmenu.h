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

char settingsBgPath[250];
char displayBgPath[250];
char aboutBgPath[250];
char developerBgPath[250];
char performanceBgPath[250];
char performance2BgPath[250];
char securityBgPath[250];
char updatesBgPath[250];
char wifiBgPath[250];
char offSwitchPath[250];
char onSwitchPath[250];

char keyBoardBgPath[250];
char cursorPath[250];

char apolloBgPath[250];
char nowplayingBgPath[250];

char fmBgPath[250];
char diriconPath[250];

char galleryBgPath[250];
char gameBgPath[250];
char quickSettingsBgPath[250];
char backiconPath[250];
char homeiconPath[250];
char multiconPath[250];
char backicon2Path[250];
char homeicon2Path[250];
char multicon2Path[250];


typedef struct fw_version fw_version;

SceUID modules[3];

int connectAPCallback(int state);
int connectToAP(int config);
char fontPath[100];
char backgroundPath[100], nickname[25], pspname[25], themeDirPath[250];
int widgetActivator, eDesktopActivator, bootAnimActivator, gBootActivator, processorState, hrTime, language, usbStat, isPlaying;
int passProtect;
int RJL;
int PSPDebug;
int updateReady;
void onlineUpdater();
void flashUpdate();
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
void themesLoad();
void themesReload();
void iconPackLoad();
void iconPackReload();
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