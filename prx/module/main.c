#include <pspsdk.h>
#include <pspkernel.h>
#include <pspsysmem_kernel.h>

PSP_MODULE_INFO("module", 0x1006, 1, 1);
PSP_MAIN_THREAD_ATTR(0);

u32 k1;

int getModuleListID(SceUID modid[100], int *modcount){
    k1 = pspSdkSetK1(0);
    sceKernelGetModuleIdList(modid, 100, modcount);
    pspSdkSetK1(k1);
    return 0;
}

int startLoadModule(char *module){
    k1 = pspSdkSetK1(0);
	SceUID modID = sceKernelLoadModule(module, 0, NULL);
	if(modID >= 0)
		sceKernelStartModule(modID, strlen(module)+1, module, NULL, NULL);
    pspSdkSetK1(k1);
	return modID;
}

int stopUnloadModule(SceUID modID){
    k1 = pspSdkSetK1(0);
    int status;
    sceKernelStopModule(modID, 0, NULL, &status, NULL);
    sceKernelUnloadModule(modID);
    pspSdkSetK1(k1);
    return 0;
}

int module_start(SceSize args, void *argp){
	return 0;
}

int module_stop(){
	return 0;
}