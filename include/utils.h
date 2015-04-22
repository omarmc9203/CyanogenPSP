#include <pspkernel.h>
#include <oslib/oslib.h>

//PSP USB
#include <pspusb.h>
#include <pspusbstor.h>
#include <pspmodulemgr.h>
#include "pspusbdevice.h"

#define downloadPath "ms0:/PSP/GAME/CyanogenMod/downloads"
#define musicPath "ms0:/MUSIC"

char tempMessage[20];
char tempPin[4];

enum colors {
    RED =	0xFF0000FF,
    GREEN =	0xFF00FF00,
    BLUE =	0xFFFF0000,
    WHITE =	0xFFFFFFFF,
    LITEGRAY = 0xFFBFBFBF,
    GRAY =  0xFF7F7F7F,
    DARKGRAY = 0xFF3F3F3F,
    BLACK = 0xFF000000
};

int fileExists(const char* path);
int dirExists(const char* path);
void deleteUpdateFile();
void makeDownloadDir();
void makeMusicDir();
void openOSK(char * initMessage, int textLimit, int lang);
void encryptFile(FILE *file);
void decryptFile(FILE *file);
int isEmpty(FILE *file);
void fadeOut(OSL_IMAGE* bg,int x, int y);
void fadeIn(OSL_IMAGE* bg, int x, int y);
int disableUsb(void);
int enableUsb();
int enableUsbEx(u32 device);
int isUSBCableConnected();

