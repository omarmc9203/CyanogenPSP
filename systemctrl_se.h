#ifndef __SCTRLLIBRARY_SE_H__
#define __SCTRLLIBRARY_SE_H__

/**
 * These functions i need for my page 4, 5 and edit menu.
*/

char hide[64], gamefolder[64], bootprog[64], noumd[64], region[64], vshmenu[64], usbdev[64], update[64], hidepic[64],
        versiontxt[64], useslimcolor[64], hidemac[64], skip[96];
		
char patch[128], bootbin[128], xmb[128], game[128], pops[128];		
		
char *regions[14] =
{
        "Disabled",
        "Japan",
        "America",
        "Europe",
        "Korea",
        "United Kingdom",
        "Mexico",
        "Australia/New Zealand",
        "East",
        "Taiwan",
        "Russia",
        "China",
        "Debug Type I",
        "Debug Type II"
};		
		
char *usbdevices[6] =
{
        "Memory Stick",
        "Flash 0",
        "Flash 1",
        "Flash 2",
        "Flash 3",
        "UMD (9660)"
};

char *umdmodes[4] =
{
        "Normal -UMD required-",
        "OE isofs legacy -NO UMD-",
        "M33 driver -NO UMD-",
        "Sony NP9660 -NO UMD-"
};

char *vshmenuoptions[3] =
{
        "Disabled",
		"VshMenu",
        "Recovery",
		"XmbConfig"
};		

/**
 * These functions are only available in SE-C and later, 
 * and they are not in HEN 
*/

/** enum for Fake Region **/
enum SEFakeRegion
{
	FAKE_REGION_DISABLED = 0,
	FAKE_REGION_JAPAN = 1,
	FAKE_REGION_AMERICA = 2,
	FAKE_REGION_EUROPE = 3,
	FAKE_REGION_KOREA = 4,
	FAKE_REGION_UNK = 5, // United Kingdom
	FAKE_REGION_UNK2 = 6, // Mexique
	FAKE_REGION_AUSTRALIA = 7,
	FAKE_REGION_HONGKONG = 8,
	FAKE_REGION_TAIWAN = 9,
	FAKE_REGION_RUSSIA = 10,
	FAKE_REGION_CHINA = 11,
	FAKE_REGION_DEBUG_TYPE_I = 12, //Dark_alex Custom région
	FAKE_REGION_DEBUG_TYPE_II = 13 //Dark_alex Custom région
};

/** enum for UMD Modes **/
enum SEUmdModes
{
	MODE_UMD = 0,
	MODE_OE_LEGACY = 1,
	MODE_MARCH33 = 2,
	MODE_NP9660 = 3
};

/** enum for USB Devices **/
enum SEUsbDevices
{
	USBDEVICE_MEMORYSTICK = 0,
	USBDEVICE_FLASH0 = 1,
	USBDEVICE_FLASH1 = 2,
	USBDEVICE_FLASH2 = 3,
	USBDEVICE_FLASH3 = 4,
	USBDEVICE_UMD9660 = 5
};

/** enum for SpeedUpMS **/
enum SESpeedUpMS
{
	SPEED_UP_NEVER = 0,
	SPEED_UP_VSH = 1,
	SPEED_UP_GAME = 2,
	SPEED_UP_VSH_AND_GAME = 3,
	SPEED_UP_POPS = 4,
	SPEED_UP_VSH_AND_POPS = 5,
	SPEED_UP_GAME_AND_POPS = 6,
	SPEED_UP_ALWAYS = 7
};

/** enum for HidePics **/
enum SEHidePics
{
	HIDE_NOTHING = 0,
	HIDE_PIC0 = 1,
	HIDE_PIC1 = 2,
	HIDE_PIC0_PIC1 = 3,
	HIDE_SND0 = 4,
	HIDE_ICON1 = 5,
	HIDE_SND0_ICON1 = 6,
	HIDE_ALL = 7
};

enum SEVshMenu
{
	VSHMENU_DISABLED = 0,
	VSHMENU_VSHMENU = 1,
	VSHMENU_RECOVERY = 2
};

/** GEN/M33 SEConfig structure **/
typedef struct
{
	int magic; // 0x47434553
	int hidecorrupt;
	int	skiplogo;
	int umdactivatedplaincheck;
	int gamekernel150;
	int executebootbin;
	int startupprog;
	int umdmode;
	int useisofsonumdinserted;
	int	vshcpuspeed;
	int	vshbusspeed;
	int	umdisocpuspeed;
	int	umdisobusspeed;
	int fakeregion;
	int freeumdregion;
	int	hardresetHB;
	int usbdevice;
	int vshmenu;
	int usbcharge;
	int notusedaxupd;
	int hidepics;
	int xmbplugins;
	int gameplugins;
	int popsplugins;
	int useversiontxt;
	int hidemac;
	int slimcolors;
} SEConfig;

/**
 * Gets the SE/OE version
 *
 * @returns the SE version
 *
 * 3.03 OE-A: 0x00000500
*/
int sctrlSEGetVersion();

/**
 * Gets the SE configuration.
 * Avoid using this function, it may corrupt your program.
 * Use sctrlSEGetCongiEx function instead.
 *
 * @param config - pointer to a SEConfig structure that receives the SE configuration
 * @returns 0 on success
*/
int sctrlSEGetConfig(SEConfig *config);

/**
 * Gets the SE configuration
 *
 * @param config - pointer to a SEConfig structure that receives the SE configuration
 * @param size - The size of the structure
 * @returns 0 on success
*/
int sctrlSEGetConfigEx(SEConfig *config, int size);

/**
 * Sets the SE configuration
 * This function can corrupt the configuration in flash, use
 * sctrlSESetConfigEx instead.
 *
 * @param config - pointer to a SEConfig structure that has the SE configuration to set
 * @returns 0 on success
*/
int sctrlSESetConfig(SEConfig *config);

/**
 * Sets the SE configuration
 *
 * @param config - pointer to a SEConfig structure that has the SE configuration to set
 * @param size - the size of the structure
 * @returns 0 on success
*/
int sctrlSESetConfigEx(SEConfig *config, int size);

/**
 * Initiates the emulation of a disc from an ISO9660/CSO file.
 *
 * @param file - The path of the 
 * @param noumd - Wether use noumd or not
 * @param isofs - Wether use the custom SE isofs driver or not
 * 
 * @returns 0 on success
 *
 * @Note - When setting noumd to 1, isofs should also be set to 1,
 * otherwise the umd would be still required.
 *
 * @Note 2 - The function doesn't check if the file is valid or even if it exists
 * and it may return success on those cases
 *
 * @Note 3 - This function is not available in SE for devhook
 * @Example:
 *
 * SEConfig config;
 *
 * sctrlSEGetConfig(&config);
 *
 * if (config.usenoumd)
 * {
 *		sctrlSEMountUmdFromFile("ms0:/ISO/mydisc.iso", 1, 1);
 * }
 * else
 * {
 *		sctrlSEMountUmdFromFile("ms0:/ISO/mydisc.iso", 0, config.useisofsonumdinserted);
 * }
*/
int sctrlSEMountUmdFromFile(char *file, int noumd, int isofs);

/**
 * Umounts an iso.
 *
 * @returns 0 on success
*/
int sctrlSEUmountUmd(void);

/**
 * Forces the umd disc out state
 *
 * @param out - non-zero for disc out, 0 otherwise
 *
*/
void sctrlSESetDiscOut(int out);

/**
 * Sets the disctype.
 *
 * @param type - the disctype (0x10=game, 0x20=video, 0x40=audio)
*/
void sctrlSESetDiscType(int type);

/**
 * Gets the current umd file (kernel only)
*/
char *sctrlSEGetUmdFile();

/**
 * Sets the current umd file (kernel only)
 *
 * @param file - The umd file
*/
void sctrlSESetUmdFile(char *file);

/** 
 * Sets the boot config file for next reboot (kernel only)
 *
 * @param index - The index identifying the file (0 -> normal bootconf, 1 -> march33 driver bootconf, 2 -> np9660 bootcnf)
*/
void sctrlSESetBootConfFileIndex(int index);

#endif

