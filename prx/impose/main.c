#include <pspsdk.h>
#include <pspkernel.h>
#include <pspimpose_driver.h>
#include <pspdisplay_kernel.h>
#include <psppower.h>
#include <pspsysmem_kernel.h>

PSP_MODULE_INFO("imposeLib", 0x1006, 1, 1);
PSP_MAIN_THREAD_ATTR(0);

u32 k1;

int imposeGetVolume(){
    k1 = pspSdkSetK1(0);
    int ret = sceImposeGetParam(PSP_IMPOSE_MAIN_VOLUME);
    pspSdkSetK1(k1);
    return ret;
}

int imposeSetVolume(int value){
    k1 = pspSdkSetK1(0);
    int ret = sceImposeSetParam(PSP_IMPOSE_MAIN_VOLUME, value);
    pspSdkSetK1(k1);
    return ret;
}

int imposeGetMute(){
    k1 = pspSdkSetK1(0);
    int ret = sceImposeGetParam(PSP_IMPOSE_MUTE);
    pspSdkSetK1(k1);
    return ret;
}

int imposeSetMute(int value){
    k1 = pspSdkSetK1(0);
    int ret = sceImposeSetParam(PSP_IMPOSE_MUTE, value);
    pspSdkSetK1(k1);
    return ret;
}

int imposeGetEqualizer(){
    k1 = pspSdkSetK1(0);
    int ret = sceImposeGetParam(PSP_IMPOSE_EQUALIZER_MODE);
    pspSdkSetK1(k1);
    return ret;
}

int imposeSetEqualizer(int value){
    k1 = pspSdkSetK1(0);
    int ret = sceImposeSetParam(PSP_IMPOSE_EQUALIZER_MODE, value);
    pspSdkSetK1(k1);
    return ret;
}

int imposeSetHomePopup(int value){
    k1 = pspSdkSetK1(0);
    int ret = sceImposeSetHomePopup(value);
    pspSdkSetK1(k1);
    return ret;
}

int imposeGetBrightness(){	
    k1 = pspSdkSetK1(0);
    int ret = sceImposeGetParam(PSP_IMPOSE_BACKLIGHT_BRIGHTNESS);
    pspSdkSetK1(k1);
    return ret;
}

int imposeSetBrightness(int value){
    k1 = pspSdkSetK1(0);
    int ret = sceImposeSetParam(PSP_IMPOSE_BACKLIGHT_BRIGHTNESS, value);
    pspSdkSetK1(k1);
    return ret;
}

int imposeGetBacklightOffTime(){	
    k1 = pspSdkSetK1(0);
    int ret = sceImposeGetBacklightOffTime();
    pspSdkSetK1(k1);
    return ret;
}

int imposeSetBacklightOffTime(int value){
    k1 = pspSdkSetK1(0);
    int ret = sceImposeSetBacklightOffTime(value);
    pspSdkSetK1(k1);
    return ret;
}

int module_start(SceSize args, void *argp){
	return 0;
}

int module_stop(){
	return 0;
}
