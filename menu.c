// Thanks to West-Li, this program is based on his PSP Module Checker.
// Thanks DAX for his ipl_update.prx
// Thanks Yoti for his libpspident.a
// Thanks Hellcat for his hc menu lib.
// Thanks Raing3 for his psppower lib.

#include <pspsdk.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define printf pspDebugScreenPrintf

extern void CheckerPrintf(char *fmt, ...);

// some vars
//////////////

char* MenuEntries[32];
int MenuEntryIds[32];
int MenuEntryCount = 0;
int MenuColorNormal;
int MenuColorSelected;


// and some functions :-)
///////////////////////////

void hcMenuClear(void)
{
    int i;

    for(i=0; i<32; i++)
    {
        MenuEntries[i] = 0;
        MenuEntryIds[i] = 0;
    }
	
    MenuEntryCount = 0;
}

void hcMenuAddEntry(char* caption, int id)
{
    MenuEntries[MenuEntryCount] = caption;
    MenuEntryIds[MenuEntryCount] = id;
    MenuEntryCount++;
}

void hcMenuSetColors(int norm, int sel)
{
    MenuColorNormal = norm;
    MenuColorSelected = sel;
}

int hcMenuShowMenu(int xpos, int ypos)
{
    int i;
    int r;
    int btn;
    SceCtrlData pad;

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);
  
    pspDebugScreenSetTextColor(MenuColorNormal);
    for(i=1; i<=MenuEntryCount; i++)
    {
        pspDebugScreenSetXY(xpos, ypos+i+1);
        printf("  %s", MenuEntries[i-1]);
    }

    r = 0; 
    i = 0;
    while(r == 0)
    {
        pspDebugScreenSetXY(xpos, ypos+i+2);
        pspDebugScreenSetTextColor(MenuColorSelected);
        printf("> %s", MenuEntries[i]);
        pspDebugScreenSetXY(0, ypos+5+MenuEntryCount);
        pspDebugScreenSetTextColor(MenuColorNormal);
	    CheckerPrintf("Press Up/Down to select an option and X to start option.\n\n");
  
        btn=1;
        while(btn != 0)
        {
            sceCtrlReadBufferPositive(&pad, 1);
            btn = pad.Buttons & 0xFFFF;
        }
        btn=0;
        while(btn == 0)
        {
            sceCtrlReadBufferPositive(&pad, 1);
            btn = pad.Buttons & 0xFFFF;
        }
        pspDebugScreenSetXY(xpos, ypos+i+2);
        pspDebugScreenSetTextColor(MenuColorNormal);
        printf("  %s", MenuEntries[i]);
        if(btn == PSP_CTRL_UP) i--; 
        if(btn == PSP_CTRL_DOWN) i++; 
        if((MenuEntryIds[i] == -1) & (btn == PSP_CTRL_UP)) i--; 
        if((MenuEntryIds[i] == -1) & (btn == PSP_CTRL_DOWN)) i++; 
        if(i > MenuEntryCount-1) i = 0; 
        if(i < 0) i = MenuEntryCount-1; 
        if(btn == PSP_CTRL_CROSS) r = MenuEntryIds[i]; 
    }

    return r;
}

