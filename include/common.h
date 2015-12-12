#include <pspsdk.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pspreg.h>
#include <kubridge.h>

//imported functions
u32 writeBat(u8 addr, u16 data);
int chWriteSerial(u16* serial);
u32 ReadEEPROM(u8 addr);
int chReadSerial(u16* pdata);
int chGetIplAttribute(int *type, int *size);
int chGetMoboVersions(int *baryon, int *pommel, int *tachyon);
int chGetVersions(int *baryon, int *pommel, int *tachyon, int *fuseid, int *fusecfg);
u32 chGetKirk();
u32 chGetSpock();
int chGetPSPHackable();
int chGetPSPCreatePandora();
int chGetCpuSpeed(int *cpu, int *bus);
int chGetFakeName(int min, int max);
int chHideMACAddress(char *file);
u8 chGetMACAddress(u8 *buf);
int pspIdStorageLookup(u16 key, u32 offset, void *buf, u32 len);
int chGetRegion();
void chInitColor(char *colorprev, char *colornext);
u32 chGetScramble();
int chDetectModel();
int chDetectOFW();
int chDetectCFW();
int chDetectMotherboard();

void exitToXMB();
void exitToGUI();
void deviceShutdown();
void deviceStandby();
void USB_Toggle();
int loadStartModule(char *module);
int stopUnloadModule(SceUID modID);
int WriteFile(char *file, void *buf, int size);
int VerifyFile(char *file);
u32 GetBatSer();
void SetBatSer(u16 ser1, u16 ser2);
int GetBatType();
int SetRegistryValue(const char *dir, const char *name, u32 val);
int GetRegistryValue(const char *dir, const char *name, u32 *val);
void swap_buttons();
void active_wma();
void active_flash();
void fake_name();
void hide_mac();
int CheckVersion(char *version);
char *GetVersion();