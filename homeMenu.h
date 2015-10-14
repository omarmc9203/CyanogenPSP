#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspsdk.h>
#include <pspctrl.h>
#include <pspsysmem.h>
#include <string.h>
#include <malloc.h> 
#include <oslib/oslib.h>
#include <psprtc.h>

#include <pspuser.h>
#include <pspgu.h>
#include <psputility.h>
#include <psputility_netmodules.h>
#include <psputility_htmlviewer.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspnet_resolver.h>
#include <psphttp.h>
#include <pspssl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/unistd.h>

//definition of our Images
OSL_IMAGE *background, *cursor, *ic_allapps, *ic_allapps_pressed, *ic_launcher_apollo, *ic_launcher_settings, *ic_launcher_messenger, *ic_launcher_browser, 
		  *pointer, *pointer1, *notif, *batt100, *batt80, *batt60, *batt40, *batt20, *batt10, *batt0, *battcharge, *backicon, *homeicon, *multicon, 
		  *transbackground, *notif2, *debug, *brightness, *control, *navbar2, *backicon2, *homeicon2, *multicon2, *welcome, *quickSettings, *playing, 
		  *navbar, *wDay, *wNight, *layerA, *layerB, *layerC, *layerD, *volumeBar, *volumeControl;
		  
//definition of our sounds
OSL_SOUND *camera_click, *KeypressStandard, *Lock, *LowBattery, *Unlock, *WirelessChargingStarted;

//definition of our font
OSL_FONT *Roboto;

void internet();
void debugDisplay();
void controls();
void battery(int batx, int baty, int n);
void volumeController();
void appDrawerIcon();
void navbarButtons(int n);
void androidQuickSettings();
void notif_2();
void loadIcons();
void unloadIcons();
void LowMemExit();
void dayNightCycleWidget();
void homeUnloadResources();
void home();