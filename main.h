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

//declaration
OSL_IMAGE * bootAnim[13];

//function declarations
int initOSLib(); 
int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks(void);
int connectAPCallback(int state);
int connectToAP(int config);
void internet();
void firstBootInitiation();
int main();