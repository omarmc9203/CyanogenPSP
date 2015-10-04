#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdisplay_kernel.h>
#include <pspimpose_driver.h>
#include <pspsysreg.h>
#include <pspsysmem_kernel.h>
#include <psppower.h>

PSP_MODULE_INFO("brightness", 0x1000, 1, 1);
PSP_MAIN_THREAD_ATTR(0);

int sceDisplayEnable(void);
int sceDisplayDisable(void);
int sceDisplayEnable371(void);
int sceDisplayDisable371(void);

int getBrightness(){
    int currentBrightness = 0;
    u32 k1; 

    k1 = pspSdkSetK1(0);
    sceDisplayGetBrightness(&currentBrightness, 0);
    pspSdkSetK1(k1);
    return currentBrightness;
}

void setBrightness(int brightness){
    u32 k1; 

    k1 = pspSdkSetK1(0);
    sceDisplaySetBrightness(brightness, 0);
    pspSdkSetK1(k1);
}

int imposeGetBrightness(){
	u32 k1; 
	
    k1 = pspSdkSetK1(0);
    int ret = sceImposeGetParam(PSP_IMPOSE_BACKLIGHT_BRIGHTNESS);
    pspSdkSetK1(k1);
    return ret;
}

int imposeSetBrightness(int value){
	u32 k1; 

    k1 = pspSdkSetK1(0);
    int ret = sceImposeSetParam(PSP_IMPOSE_BACKLIGHT_BRIGHTNESS, value);
    pspSdkSetK1(k1);
    return ret;
}

int displayEnable(void){
    u32 k1 = pspSdkSetK1(0);
    int ret;
    if (sceKernelDevkitVersion() < 0x03070110)
        ret = sceDisplayEnable();
    else
        ret = sceDisplayEnable371();
    pspSdkSetK1(k1);
    return ret;
}

int displayDisable(void){
    u32 k1 = pspSdkSetK1(0);
	int ret;
    if (sceKernelDevkitVersion() < 0x03070110)
        ret = sceDisplayDisable();
    else
        ret = sceDisplayDisable371();
    pspSdkSetK1(k1);
    return ret;
}

int module_start(SceSize args, void *argp) 
{
	return 0;
}

int module_stop()
{
	return 0;
}
