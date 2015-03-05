#include <pspkernel.h>
#include <pspctrl.h>
#include <pspumd.h>

//PSP Net Stuff
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>

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

//declaration
OSL_IMAGE *settingsbg, *cursor, *usbdebug, *aboutbg, *offswitch, *onswitch, *displaybg, *performancebg, *wifibg, *developerbg, *about, *highlight, 
		  *developeroptions, *themes, *wifi, *processorbg, *cpuset, *check, *updatesbg, *performance, *recoverybg, *easterEggImg, *security;

//definition of our sounds
OSL_SOUND *tone;

OSL_FONT *Roboto;

typedef struct fw_version;

char backgroundPath[100];
int widgetActivator;
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
void processorMenu();
void ramMenu();
void displayMenu();
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