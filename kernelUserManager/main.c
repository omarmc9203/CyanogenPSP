// Thanks to West-Li, this program is based on his PSP Module Checker.
// Thanks DAX for his ipl_update.prx
// Thanks Yoti for his libpspident.a
// Thanks Hellcat for his hc menu lib.
// Thanks Raing3 for his psppower lib.

#include <pspkernel.h>
#include <pspsysmem_kernel.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "pspipl_update.h"

PSP_MODULE_INFO("KernelUserManager", 0x1006, 2, 4);
PSP_MAIN_THREAD_ATTR(0);

static u8 orig_ipl[0x24000] __attribute__((aligned(64)));

u64 sceSysreg_driver_4F46EEDE(void);        // FuseID
u32 sceSysreg_driver_8F4F4E96(void);        // FuseCFG 
u32 sceSyscon_driver_7EC5A957(u32 *val);	// Baryon
u32 sceSyscon_driver_E7E87741(u32 *val);	// Pommel
u32 sceSysreg_driver_E2A5D1EE(void);		// Tachyon

SceUID fd;

int chGetIplAttribute(int *type, int *size)
{
	int ipl, size_;
	size_ = pspIplUpdateGetIpl(orig_ipl);
	if(size_ == 0x24000) ipl = 0;
	else if(size_ == 0x20000) ipl = 1;
    else ipl = 2;	
	
	(*type) = ipl;
	(*size) = size_;
}

int chGetMoboVersions(int *baryon, int *pommel, int *tachyon)
{
	u32 verb, verp, vert;
	
	int k1 = pspSdkSetK1(0);
	sceSyscon_driver_7EC5A957(&verb);
	sceSyscon_driver_E7E87741(&verp);
    vert = sceSysreg_driver_E2A5D1EE();
	pspSdkSetK1(k1);
	
	(*baryon) = verb;
	(*pommel) = verp;
    (*tachyon) = vert;
}

int chGetVersions(int *baryon, int *pommel, int *tachyon, int *fuseid, int *fusecfg)
{
	int pagesize, ppb, totalblocks;
	u32 verb, verp, vert, cfg, nand;
	u64 id;

	int k1 = pspSdkSetK1(0);
	sceSyscon_driver_7EC5A957(&verb);
	sceSyscon_driver_E7E87741(&verp);
    vert = sceSysreg_driver_E2A5D1EE();
	id = sceSysreg_driver_4F46EEDE();
	cfg = sceSysreg_driver_8F4F4E96(); 
	pspSdkSetK1(k1);
	
	(*baryon) = verb;
	(*pommel) = verp;
    (*tachyon) = vert;
	(*fuseid) = id;
	(*fusecfg) = cfg;
}

u32 chGetKirk() //raing3
{ 
   int k1 = pspSdkSetK1(0);

   int ver, i = 0, val = *(u32 *)0xBDE00004;
   sceSysregKirkBusClockEnable();
   
   do
   {
       i++;
       sceKernelDelayThread(1000);
       ver = *(u32 *)0xBDE00004;
   }
   
   while ((ver == val) && (i < 10));
   
   pspSdkSetK1(k1);
   
   return ver;
}

u32 chGetSpock() //raing3
{ 
   int k1 = pspSdkSetK1(0);

   int ver, i = 0, val = *(u32 *)0xBDF00004;
   sceSysregAtaBusClockEnable();
   
   do
   {
       i++;
       sceKernelDelayThread(1000);
       ver = *(u32 *)0xBDF00004;
   }
   
   while ((ver == val) && (i < 10));
   
   pspSdkSetK1(k1);
   
   return ver;
}

//Simple algorithm
int chGetPSPHackable()
{
	int baryon, pommel, tachyon, model;
	
	chGetMoboVersions(&baryon, &pommel, &tachyon);
	
	model = sceKernelGetModel();
	
	if(model == 0) return 0; //PSP FAT (All PSP FAT is hackable so we put yes)
	else if(model == 1 && tachyon == 0x00600000 && baryon == 0x00243000) return 1; //PSP SLIM TA-088v3 (No hackable so we put No)
	else if(model == 1 && tachyon < 0x00600000) return 0; //PSP SLIM < than TA-088v3 (Its hackable so we put Yes)
	else if(model > 1) return 1; //PSP > than SLIM (No hackable so we put No)
	else return 2; //Sets Hackable to "Unknown" if cant get
}

int chGetPSPCreatePandora()
{
	int baryon, pommel, tachyon;
	
	chGetMoboVersions(&baryon, &pommel, &tachyon);
	
	if(baryon < 0x00234000) return 0;
	if(baryon > 0x0022B200) return 1;
	else return 2; //Sets CreatePandora to "Unknown" if cant get
}

int chGetCpuSpeed(int *cpu, int *bus)
{
    int cpu_speed, bus_speed;
	
	cpu_speed = scePowerGetCpuClockFrequency();
	bus_speed = scePowerGetBusClockFrequency();
	
	(*cpu) = cpu_speed;
    (*bus) = bus_speed;
}

int chGetFakeName(int min, int max)
{
	u64 tick;
	SceKernelUtilsMt19937Context ctx;
	sceRtcGetCurrentTick(&tick);
	sceKernelUtilsMt19937Init(&ctx, (u32)tick);

	return min + (sceKernelUtilsMt19937UInt(&ctx) % max);
}

int chHideMACAddress(char *file) 
{
    fd = sceIoOpen(file, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	
	if(fd < 0)
	{  
	    sceIoClose(fd);
		return -1;
    }	
	else
	{
	    sceIoClose(fd);
		return 0;
	}
}

u8 chGetMACAddress(u8 *buf) //raing3
{
	sceWlanGetEtherAddr(buf);
	return buf;
}

int pspIdStorageLookup(u16 key, u32 offset, void *buf, u32 len) //Chily willy
{
	int k1 = pspSdkSetK1(0);
	memset(buf, 0, len);
	int ret = sceIdStorageLookup(key, offset, buf, len);
	pspSdkSetK1(k1);
}

int chGetRegion() //raing3
{
	u8 region[1];
	pspIdStorageLookup(0x100, 0x3D, region, 1);

	if(region[0] == 0x03) return 0;
	else if(region[0] == 0x04) return 1;
	else if(region[0] == 0x09) return 2;
	else if(region[0] == 0x07) return 3;
	else if(region[0] == 0x05) return 4;
	else if(region[0] == 0x06) return 5;
	else if(region[0] == 0x0A) return 6;
	else if(region[0] == 0x0B) return 7;
	else if(region[0] == 0x0C) return 8;
	else if(region[0] == 0x0D) return 9;
	else return 10;
}

void chInitColor(char *colorprev, char *colornext) 
{
	sceIoRemove(colorprev);
	fd = sceIoOpen(colornext, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	sceIoClose(fd);
}

u32 chGetScramble() //raing3
{
	int k1 = pspSdkSetK1(0);

	u32 magic;
	u32 buf[4];
	u32 sha[5];

	buf[0] = *(vu32*)(0xBC100090);
	buf[1] = *(vu32*)(0xBC100094);
	buf[2] = *(vu32*)(0xBC100090)<<1;
	buf[3] = 0xD41D8CD9;

	sceKernelUtilsSha1Digest((u8*)buf, sizeof(buf), (u8*)sha);

	magic = (sha[0] ^ sha[3]) + sha[2];

	pspSdkSetK1(k1);

	return magic;
}

int chDetectModel()
{
	int baryon, pommel, tachyon, model;
	
	model = sceKernelGetModel();
	chGetMoboVersions(&baryon, &pommel, &tachyon);
	
	if(model == 0) return 0;
	else if(model == 1) return 1;
	else if(model == 2) return 2;
	else if(model == 3) return 3;
	else if(model == 4) return 4;
	else if(model == 7) return 5;
	else if(model == 9) return 6;
	
	//Detect E-1000
	//NOTE: We dont know the number of E-1000 but we know MB so we use tachyon, baryon, pommel to detect E-1000
	else if (tachyon == 0x00900000 && baryon == 0x00403000 && pommel == 0x00000154) return 7;
	else return 8; //Sets PSP to "Unknown" if cant detect
}

int chDetectOFW()
{
	int fw_version = sceKernelDevkitVersion();
	
	if(fw_version == 0x06060010) return 0;
	else if(fw_version == 0x06030910) return 1;
	else if(fw_version == 0x06030810) return 2;
	else if(fw_version == 0x06030710) return 3;
	else if(fw_version == 0x06030510) return 4;
	else if(fw_version == 0x06020010) return 5;
	else if(fw_version == 0x05050010) return 6;
	else if(fw_version == 0x05000310) return 7;
	else if(fw_version == 0x05000010) return 8;
	else return 9; //Sets FW to "Unknown" if cant detect
}

//Edited by me (code from psp everest)
int chDetectCFW()
{
	int henid = sctrlHENGetVersion(), fw_version = sceKernelDevkitVersion();

	if(fw_version == 0x05000010) return 0;
	else if(fw_version == 0x05000210) return 1;
	else if(fw_version == 0x05000310) return 2;
	else if(fw_version == 0x05050010) return 1;
	else if(fw_version == 0x06030110) return 3;
	else if(fw_version == 0x06030610) return 3;
	else if(fw_version > 0x06000010 && fw_version < 0x06080010) //6.00, 6.80 = non-reals fw
	{
		if(sctrlHENGetMinorVersion() != 0x8002013A)
		{
			if(henid == 0x00001001) return 4;
			else if(henid == 0x00001002) return 5;
			else if(henid == 0x00001003) return 6;
		}
		
		else if(henid == 0x00001000 && fw_version == 0x06020010) return 7;
		else if(henid == 0x00001001 && fw_version == 0x06020010) return 8;
		else if(henid == 0x00001002 && fw_version == 0x06020010) return 9;
		else if(henid == 0x00001003 && fw_version == 0x06020010) return 10;
		else if(henid == 0x00001004 && fw_version == 0x06020010) return 11;
		else if(henid == 0x00001000 && fw_version == 0x06030510) return 12;
		else if(henid == 0x00001000 && fw_version == 0x06030710) return 13;
	    else if(henid == 0x00001000 && fw_version == 0x06030810) return 13;
		else if(henid == 0x00001000 && fw_version == 0x06030910) return 13;
		else if(henid == 0x00002000 && fw_version == 0x06030910) return 7;
		else if(henid == 0x00001000 && fw_version == 0x06060010) return 13;
	}
	
	else return 14;
}

//Added detect TA-082, TA-096, TA-094 and more, more...
int chDetectMotherboard()
{
	int baryon, pommel, tachyon;
	
	chGetMoboVersions(&baryon, &pommel, &tachyon);
	
	if(tachyon == 0x00140000) return 0;
	else if(tachyon == 0x00200000 && baryon == 0x00030600) return 1;
	else if(tachyon == 0x00200000 && baryon == 0x00040600) return 2;
	else if(tachyon == 0x00300000 && baryon == 0x00040600 && pommel == 0x00000103) return 3;
	else if(tachyon == 0x00300000 && baryon == 0x00040600 && pommel == 0x00000104) return 4;
	else if(tachyon == 0x00400000 && baryon == 0x00114000) return 5;
	else if(tachyon == 0x00400000 && baryon == 0x00121000) return 6;
	else if(tachyon == 0x00500000 && baryon == 0x0022B200) return 7;
	else if(tachyon == 0x00500000 && baryon == 0x00234000) return 8;
	else if(tachyon == 0x00500000 && baryon == 0x00243000) return 9;
	else if(tachyon == 0x00600000 && baryon == 0x00243000) return 10;
	else if(tachyon == 0x00500000 && baryon == 0x00243000 && pommel == 0x00000132) return 11;
	else if(tachyon == 0x00600000 && baryon == 0x00263100 && pommel == 0x00000132) return 12;
	else if(tachyon == 0x00600000 && baryon == 0x00263100 && pommel == 0x00000133) return 13;
	else if(tachyon == 0x00600000 && baryon == 0x00285000) return 14;
	else if(tachyon == 0x00810000 && baryon == 0x002C4000 && pommel == 0x00000141) return 15;
	else if(tachyon == 0x00810000 && baryon == 0x002C4000 && pommel == 0x00000143) return 16;
	else if(tachyon == 0x00810000 && baryon == 0x002E4000) return 17;
	else if(tachyon == 0x00820000 && baryon == 0x002E4000) return 18;
	else if(tachyon == 0x00720000 && baryon == 0x00304000) return 19;
	else if(tachyon == 0x00800000 && baryon == 0x002A0000) return 20;
	else if(tachyon == 0x00900000 && baryon == 0x00403000) return 21;
	else if(tachyon == 0x00100000) return 22;  
	else return 23; //Sets MB to "Unknown" if cant detect
}

int module_start(SceSize args, void *argp)
{
	return 0;
}

int module_stop()
{
	return 0;
}