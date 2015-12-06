// Thanks to West-Li, this program is based on his PSP Module Checker.
// Thanks DAX for his ipl_update.prx
// Thanks Yoti for his libpspident.a
// Thanks Hellcat for his hc menu lib.
// Thanks Raing3 for his psppower lib.

#include <pspkernel.h>
#include <psppower.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

PSP_MODULE_INFO("BatteryManager", 0x1006, 1, 4);
PSP_MAIN_THREAD_ATTR(0);

/* 
	Battery (code from Open Source Pandora Battery Tool by cory1492)
*/
u32 writeBat(u8 addr, u16 data)
{
	int k1 = pspSdkSetK1(0);

	int res;
	u8 param[0x60];

	if(addr > 0x7F)
		return(0x80000102);

	param[0x0C] = 0x73;
	param[0x0D] = 5;
	param[0x0E] = addr;
	param[0x0F] = data;
	param[0x10] = data >> 8;

	res = sceSysconCmdExec(param, 0);

	if(res < 0)
		return res;

	pspSdkSetK1(k1);

	return 0;
}

int chWriteSerial(u16* serial)
{
	int err = 0;

	err = writeBat(0x07, serial[0]);
	if(!err)
		err = writeBat(0x09, serial[1]);

	return err;
}

u32 ReadEEPROM(u8 addr)
{
	u8 param[0x60];

	if(addr > 0x7F)
		return 0x80000102;
		
	param[0x0C] = 0x74;
	param[0x0D] = 3;
	param[0x0E] = addr;

	int res = sceSysconCmdExec(param, 0);
	if(res < 0)
		return res;

	return (param[0x21] << 8) | param[0x20];
}

int chReadSerial(u16* pdata)
{
	int err = 0;
	u32 data;

	u32 k1 = pspSdkSetK1(0);

	int errCheck(u32 chdata)
	{
		if((chdata & 0x80250000) == 0x80250000)
			return -1;
		else if(chdata & 0xFFFF0000)
			return(chdata & 0xFFFF0000) >> 16;

		return 0;
	}

	data = ReadEEPROM(0x07);	
	err = errCheck(data);

	if(err >= 0)
	{
		pdata[0] = (data & 0xFFFF);
		data = ReadEEPROM(0x09);
		err = errCheck(data);
		if(err >= 0)
			pdata[1] = (data & 0xFFFF);
		else
			err = data;
	}
	else
		err = data;

	pspSdkSetK1(k1);

	return err;
}

int module_start(SceSize args, void *argp)
{
	return 0;
}

int module_stop()
{
	return 0;
}