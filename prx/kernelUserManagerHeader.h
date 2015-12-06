#ifndef __KUMAN_DRIVER_H__
#define __KUMAN_DRIVER_H__

char *MBs[] =
{ 
	//Fat
	
	"TA-079 v1", 
	"TA-079 v2", 
	"TA-079 v3", 
	"TA-081 v1", 
	"TA-081 v2", 
	"TA-082", 
	"TA-086",
	
	//Slim
	
	"TA-085", 
	"TA-085 v2", 
	"TA-088 v1/v2", 
	"TA-088 v3", 
	"TA-090",
	
	//Brite
	
	"TA-090 v2",
    "TA-090 v3",	
	"TA-092", 
	"TA-093 v1",
	"TA-093 v2",
	"TA-095 v1",
	"TA-095 v2",
	
	//Go
	
	"TA-091", 
	"TA-094", 
	
	//E-1000
	"TA-096",
	
	//Unknown, devkit
	
	"Devkit", 
	"Unknown"
};

char *PSPs[] =
{ 
	"PSP Fat 100",
	"PSP Slim 200",
	"PSP Brite 300",
	"PSP Brite 400",
	"PSP GO! N1004",
	"PSP Brite 700",
	"PSP Brite 900",
	"PSP Street ",
	"Unknown"
};

char *Modules[] =
{ 
	"01g",
	"02g",
	"03g",
	"04g",
	"05g",
	"07g",
	"09g",
	"Unknown"
};

char *PSPUtils[] =
{ 
	"Yes",
	"No",
	"Unknown"
};

char *FWs[] =
{ 
	"6.60",
	"6.39",
	"6.38",
	"6.37",
	"6.35",
	"6.20",
	"5.50",
	"5.03",
	"5.00",
	"Unknown"
};

char *Regions[] =
{ 
	"Japan",
	"America",
	"Australia",
	"United Kingdom",
	"Europe",
	"Korea",
	"Hong Kong",
	"Taiwan",
	"Russia",
	"China",
	"Unknown"
};

char *CFWs[] =
{ 
	"M33",
	"GEN",
	"GEN/MHU",
	"PRO (HEN)",
	"PRO-A",
	"PRO-B",
	"PRO-C",
	"TN-A",
	"TN-B",
	"TN-C",
	"TN-D",
	"TN-E",
	"Custom",
	"ME",
	"Unknown"
};

char *IPLs[] =
{ 
	"Custom Ipl",
	"RAW Ipl",
	"Unknown"
};

char *Batterys[] =
{ 
	"Service Mode",
	"AutoBoot",
	"Normal"
};

int chGetIplAttribute(int *type, int *size);
int chGetVersions(int *baryon, int *pommel, int *tachyon, int *fuseid, int *fusecfg);
u32 chGetKirk();
u32 chGetSpock();
int chGetPSPHackable();
int chGetPSPCreatePandora();
int chGetCpuSpeed(int *cpu, int *bus);
int chGetFakeName(int min, int max);
int chHideMACAddress(char *file);
u8 chGetMACAddress(u8 *buf);
void chInitColor(char *colorprev, char *colornext);
u32 chGetScramble();
int chDetectModel();
int chDetectOFW();
int chDetectCFW();
int chDetectMotherboard();

#endif