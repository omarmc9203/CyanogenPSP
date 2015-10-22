#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspsdk.h>
#include <pspctrl.h>
#include <pspsysmem.h>
#include <string.h>
#include <malloc.h> 

//OSLib
#include <oslib/oslib.h>

//PSP Time
#include <psprtc.h>

#define MAX_LANG_DISPLAY		3
#define LANG_DISPLAY_X			240
#define LANG_DISPLAY_Y			157 

//declaration
OSL_IMAGE * bootAnim[13], *langSelection;

//function declarations
int initOSLib(); 
int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks(void);
void langDisplay();
void langScrollUp();
void langScrollDown();
void langControls();
char * langBrowse(const char * path);
void displayLangSelection(char * browseDirectory);
int spawnLangSelection();
void firstBootInitiation();
int main();