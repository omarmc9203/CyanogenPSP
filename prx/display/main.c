#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdisplay_kernel.h>
#include <pspsysreg.h>
#include <pspsysmem_kernel.h>
#include <psppower.h>

PSP_MODULE_INFO("display", 0x1000, 1, 1);
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

int module_start(SceSize args, void *argp){
	return 0;
}

int module_stop(){
	return 0;
}
