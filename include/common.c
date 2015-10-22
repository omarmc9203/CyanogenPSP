// Thanks to West-Li, this program is based on his PSP Module Checker.
// Thanks DAX for his ipl_update.prx
// Thanks Yoti for his libpspident.a
// Thanks Raing3 for his psppower lib.
#include "common.h"
#include "utils.h"
#include "../homeMenu.h"

char version_txt[256];
char *file;

u32 value;

void exitToXMB()
{
	sceIoRemove("ipl_update.prx");
	sceIoRemove("batman.prx");
	sceIoRemove("kuman.prx");
	sceKernelExitGame();
}

void exitToGUI()
{
	sceIoRemove("ipl_update.prx");
	sceIoRemove("batman.prx");
	sceIoRemove("kuman.prx");
	home();
}

void deviceShutdown()
{
	scePowerRequestStandby();
}

void deviceStandby()
{
	scePowerRequestSuspend();
}

void USB_Toggle()
{	
	SceCtrlData pad;
	
	if(pad.Buttons & PSP_CTRL_SELECT)
	{
		enableUsb();
	}
		
	else if(pad.Buttons & PSP_CTRL_SELECT)
	{
		disableUsb();
	}
}

int loadStartModule(char *module)
{
	SceUID modID = sceKernelLoadModule(module, 0, NULL);
	if(modID >= 0)
		sceKernelStartModule(modID, strlen(module)+1, module, NULL, NULL);
	return modID;
}

int stopUnloadModule(SceUID modID)
{
    int status;
    sceKernelStopModule(modID, 0, NULL, &status, NULL);
    sceKernelUnloadModule(modID);
    return 0;
}

//File helper
int WriteFile(char *file, void *buf, int size)
{
	SceUID fd = sceIoOpen(file, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	
	if (fd < 0)
	{
		return fd;
	}

	int written = sceIoWrite(fd, buf, size);
	
	if(written < 0)
	   oslDrawStringf(10,210,"Cannot write %s", file);

	sceIoClose(fd);
	
	return 0;
}

//Check File
int VerifyFile(char *file) 
{
	SceUID fd = sceIoOpen(file, PSP_O_RDONLY, 0777);
	
	if(fd < 0)
	   return -1;

	sceIoClose(fd);

	return 0;
}

//Battery
//By raing3 
u32 GetBatSer()
{
	u16 BatSer[2];
	chReadSerial(BatSer);

	return (BatSer[1] & 0xFFFF) + ((BatSer[0] & 0xFFFF)*0x10000);
}

void SetBatSer(u16 ser1, u16 ser2)
{
	if(GetBatSer() == (ser2 & 0xFFFF) + ((ser1 & 0xFFFF)*0x10000)) oslDrawStringf(10,210, "The current battery serial is already set to 0x%04X%04X. There is no need to change the battery serial.", ser1, ser2);

	u16 BatSer[2];
	BatSer[0] = ser1;
	BatSer[1] = ser2;

	if((BatSer[0] == 0x5241)&&(BatSer[1] == 0x4E44))
	{ // generate a random serial if the serial passed is RAND
		BatSer[0] = chGetFakeName(0x0001, 0xFFFE); //Fake name create random serial too
		BatSer[1] = chGetFakeName(0x0001, 0xFFFE); //Fake name create random serial too
	}
	
	chWriteSerial(BatSer);

	if(GetBatSer() != (BatSer[1] & 0xFFFF) + ((BatSer[0] & 0xFFFF)*0x10000)) oslDrawStringf(10,210, "Unable to write to the Battery EEPROM.");
}

int GetBatType()
{
	u32 BatSer = GetBatSer();
	
	if(BatSer == 0xFFFFFFFF) return 0;
	else if(BatSer == 0x00000000) return 1;
	else return 2;
}

//Registry
int SetRegistryValue(const char *dir, const char *name, u32 val)
{
    int ret = 0;
    struct RegParam reg;
    REGHANDLE h;

    memset(&reg, 0, sizeof(reg));
    reg.regtype = 1;
    reg.namelen = strlen("/system");
    reg.unk2 = 1;
    reg.unk3 = 1;
    strcpy(reg.name, "/system");
    if(sceRegOpenRegistry(&reg, 2, &h) == 0)
    {
        REGHANDLE hd;
        if(!sceRegOpenCategory(h, dir, 2, &hd))
        {
            if(!sceRegSetKeyValue(hd, name, &val, 4))
            {
                ret = 1;
                sceRegFlushCategory(hd);
            }
                        else
                        {
                                sceRegCreateKey(hd, name, REG_TYPE_INT, 4);
                                sceRegSetKeyValue(hd, name, &val, 4);
                                ret = 1;
                sceRegFlushCategory(hd);
                        }
            sceRegCloseCategory(hd);
        }
        sceRegFlushRegistry(h);
        sceRegCloseRegistry(h);
    }

        return ret;
}

int GetRegistryValue(const char *dir, const char *name, u32 *val) 
{
    int ret = 0;
    struct RegParam reg;
    REGHANDLE h;

    memset(&reg, 0, sizeof(reg));
    reg.regtype = 1;
    reg.namelen = strlen("/system");
    reg.unk2 = 1;
    reg.unk3 = 1;
    strcpy(reg.name, "/system");
    if(sceRegOpenRegistry(&reg, 2, &h) == 0) {
        REGHANDLE hd;
        if(!sceRegOpenCategory(h, dir, 2, &hd)) {
            REGHANDLE hk;
            unsigned int type, size;

            if(!sceRegGetKeyInfo(hd, name, &hk, &type, &size)) {
                if(!sceRegGetKeyValue(hd, hk, val, 4)) {
                    ret = 1;
                    sceRegFlushCategory(hd);
                }
            }
            sceRegCloseCategory(hd);
        }
        sceRegFlushRegistry(h);
        sceRegCloseRegistry(h);
    }

    return ret;
}

//Registry Func
void swap_buttons()
{
	GetRegistryValue("/CONFIG/SYSTEM/XMB", "button_assign", &value);
	value = !value;
    SetRegistryValue("/CONFIG/SYSTEM/XMB", "button_assign", value); 

    if(value) 
    {
        oslDrawStringf(10,210, "Done!, Enter button is now X");		
	} 
	else 
	{
        oslDrawStringf(10,210, "Done!, Enter button is now O");	
    }
}

void active_wma()
{
	GetRegistryValue("/CONFIG/MUSIC", "wma_play", &value);

	if (value == 1)
	{
		oslDrawStringf(10,210, "WMA is already activated.");
	}
	else
	{
		SetRegistryValue("/CONFIG/MUSIC", "wma_play", 1);
		oslDrawStringf(10,210, "WMA activated.");
	}
}

void active_flash()
{
	GetRegistryValue("/CONFIG/BROWSER", "flash_activated", &value);

	if(value == 1)
	{
		oslDrawStringf(10,210, "Flash player is already activated.");
	}
	else
	{
		SetRegistryValue("/CONFIG/BROWSER", "flash_activated", 1);
        SetRegistryValue("/CONFIG/BROWSER", "flash_play", 1);
	    oslDrawStringf(10,210, "Flash player activated.");
	}
}

void fake_name()
{
	oslDrawStringf(10,210, "Setting random name...");
	value = chGetFakeName(20, 3465);
	SetRegistryValue("/CONFIG/SYSTEM", "owner_name", value);
    sceKernelDelayThread(2000000);
	oslDrawStringf(10,220, "Done");
}

void hide_mac()
{
	oslDrawStringf(10,210, "Hidding MAC address...");
	
	int hidemac = chHideMACAddress("machide.chk");
	if(hidemac == -1) 
		oslDrawStringf(10,220, "Cannot hide MAC address"); 
	
	sceKernelDelayThread(2000000);
	oslDrawStringf(10,220, "Done");
}

//Version.txt
int CheckVersion(char *version)
{
	SceUID fd = sceIoOpen(version, PSP_O_RDONLY, 0777);
	
	if(fd < 0)
	   return -1;

	sceIoClose(fd);
	
	file = version;

	return 0;
}

char *GetVersion()
{
	memset(version_txt, 0, sizeof(version_txt));
	SceUID fd = sceIoOpen(file, PSP_O_RDONLY, 0777);
	
	sceIoRead(fd, version_txt, 255);

	sceIoClose(fd);

	return version_txt;
}
