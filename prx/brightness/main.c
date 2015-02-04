#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdisplay_kernel.h>

PSP_MODULE_INFO("brightness", 0x1000, 1, 1);
PSP_MAIN_THREAD_ATTR(0);

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

int module_start(SceSize args, void *argp) 
{
	return 0;
}

int module_stop()
{
	return 0;
}
