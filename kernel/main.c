/*
  Kernel memory Hook
*/

#include <pspsdk.h>
#include <pspkernel.h>
#include <pspsysevent.h>
#include <pspsysmem.h>
#include <pspsysmem_kernel.h>

#include <psptypes.h>

#include <stdio.h>
#include <string.h>

PSP_MODULE_INFO("SystemHook", PSP_MODULE_KERNEL, 1, 0);

SceSize sysKernelPartitionMaxFreeMemSize(int pid)
{
    int k1 = pspSdkSetK1(0);
    SceSize ret = sceKernelPartitionMaxFreeMemSize(pid);
    pspSdkSetK1(k1);
    return ret;
}


int module_start(SceSize args, void *argp)
{
	return 0;
}

int module_stop(SceSize args, void *argp)
{
	return 0;
}