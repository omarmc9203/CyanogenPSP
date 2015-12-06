#include <pspkernel.h>  
#include <pspdebug.h> 
#include <pspctrl.h>
#include <psppower.h>
#include <stdio.h>
#include <pspwlan.h>
#include <oslib/oslib.h>

#include <kubridge.h>

#define Version "flash0:/vsh/etc/version.txt"
#define PWD "password.TXT"
#define selector_xDistance 0 //The distance between the X axis of the two selections
#define selector_yDistance 20 //The distance between the Y axis of the two selections

int scePowerGetBatteryRemainCapacity(void);

OSL_IMAGE *recoverybg, *Selector;

OSL_FONT *roboto;

//Imports
u32 chGetKirk();
u32 chGetSpock();

void ShowPage5();
void ShowPage4();
void backupPassword();
void ShowPage3();
void ShowPage2();
void ShowPage1();
void ShowVersionTxt();
int ConfigurationMenu();
int ShowAdvancedCnfMenu();
int ShowCnfMenu();
int ShowSystemMenu();
int ShowBatteryMenu();
void unloadRecoveryMenuRes();
int mainRecoveryMenu();