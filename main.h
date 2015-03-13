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

#define ADDRESS "www.google.com"

//declaration
		  
OSL_FONT *Roboto;

static int runningFlag = 1;
static char message[100] = "";
static char buffer[100] = "";

//function declarations
int initOSLib(); 
int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks(void);
int connectAPCallback(int state);
int connectToAP(int config);
void internet();
void firstBootMessage();