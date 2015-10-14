#include <pspkernel.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <oslib/oslib.h>

//PSP USB
#include <pspusb.h>
#include <pspusbstor.h>
#include <pspmodulemgr.h>
#include <kubridge.h>
#include "pspusbdevice.h"

#define downloadPath "ms0:/PSP/GAME/CyanogenMod/downloads"
#define musicPath "ms0:/MUSIC"

char tempMessage[20];
char tempPin[5];
char tempData[250];

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

int setFileDefaultsInt(char *path, int value, int var);
void setFileDefaultsChar(char *path, char* data, char* var);
void deleteUpdateFile();
void createDirs();
void makeMusicDir();
void openOSK(char * message, char * initialMessage, int textLimit, int lang);
void encryptFile(FILE *file);
void decryptFile(FILE *file);
int isEmpty(FILE *file);
void fadeOut(OSL_IMAGE* bg,int x, int y);
void fadeIn(OSL_IMAGE* bg, int x, int y);
int disableUsb(void);
int enableUsb();
int enableUsbEx(u32 device);
char getPSPNickname();
int isUSBCableConnected();

