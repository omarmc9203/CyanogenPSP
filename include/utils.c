#include "utils.h"
#include "../home.h"
#include "../settingsmenu.h"
#include "../clock.h"
#include "../screenshot.h"


int transp;//Fo transparency
int fade;//To hold the colour of the rectangle blitted

char usbStatus = 0;
char usbModuleStatus = 0;

int fileExists(const char* path)
{
	SceUID dir = sceIoOpen(path, PSP_O_RDONLY, 0777);
	if (dir >= 0)
	{
		sceIoClose(dir);
		return 1;
	}
	else
	{
		return 0;
	}
}

int dirExists(const char* path)
{
	SceUID dir = sceIoDopen(path);
	if (dir >= 0)
	{
		sceIoDclose(dir);
		return 1;
	}
	else 
	{
		return 0;
	}
}

char* ReadFile(char *filename)
{
   char *buffer = NULL;
   int string_size,read_size;
   FILE *handler = fopen(filename,"r");

   if (handler)
   {
       //seek the last byte of the file
       fseek(handler,0,SEEK_END);
       //offset from the first to the last byte, or in other words, filesize
       string_size = ftell (handler);
       //go back to the start of the file
       rewind(handler);

       //allocate a string that can hold it all
       buffer = (char*) malloc (sizeof(char) * (string_size + 1) );
       //read it all in one operation
       read_size = fread(buffer,sizeof(char),string_size,handler);
       //fread doesnt set it so put a \0 in the last position
       //and buffer is now officialy a string
       buffer[string_size] = '\0';

       if (string_size != read_size) 
	   {
           //something went wrong, throw away the memory and set
           //the buffer to NULL
           free(buffer);
           buffer = NULL;
        }
    }
	fclose(handler);
    return buffer;
}

int setFileDefaultsInt(char *path, int value, int var)
{
	FILE *temp;
	 
	if (!(fileExists(path)))
	{
		temp = fopen(path, "w");
		fprintf(temp, "%d", value);
		fclose(temp);
	} 
	
	temp = fopen(path, "r");
	fscanf(temp, "%d", &var);
	fclose(temp);
	
	return var;
}

void setFileDefaultsChar(char *path, char* data, char* var)
{
	FILE * temp;
	
	if (!(fileExists(path)))
	{
		temp = fopen(path, "w");
		fprintf(temp, "%s", data);
		fclose(temp);
	}
	
	temp = fopen(path, "r");
	fscanf(temp, "%s", var);
	fclose(temp);
}

void deleteUpdateFile()
{
	if (fileExists("ms0:/PSP/GAME/update.zip"))
	{
		sceIoRemove("ms0:/PSP/GAME/update.zip");
	}
}

void createDirs()
{
	SceUID dir = sceIoDopen("ms0:/PICTUREO");
	
	if (dirExists("ms0:/PICTURE"))
	{
		sceIoDclose(dir);
	}
	else 
	{
		sceIoMkdir("ms0:/PICTURE",0777);
	}

	SceUID dir1 = sceIoDopen("ms0:/PSP/PHOTO");
	
	if (dirExists("ms0:/PSP/PHOTO"))
	{
		sceIoDclose(dir1);
	}
	else 
	{
		sceIoMkdir("ms0:/PSP/PHOTO",0777);
	}
	
	SceUID dir2 = sceIoDopen("ms0:/PSP/GAME/CyanogenPSP/screenshots");
	
	if (dirExists("ms0:/PSP/GAME/CyanogenPSP/screenshots"))
	{
		sceIoDclose(dir2);
	}
	else 
	{
		sceIoMkdir("ms0:/PSP/GAME/CyanogenPSP/screenshots",0777);
	}
	
	SceUID dir3 = sceIoDopen("ms0:/MUSIC");
	
	if (dirExists("ms0:/MUSIC"))
	{
		sceIoDclose(dir3);
	}
	else 
	{
		sceIoMkdir("ms0:/MUSIC",0777);
	}
	
	SceUID dir4 = sceIoDopen("ms0:/PSP/MUSIC");
	
	if (dirExists("ms0:/PSP/MUSIC"))
	{
		sceIoDclose(dir4);
	}
	else 
	{
		sceIoMkdir("ms0:/PSP/MUSIC",0777);
	}
	
	SceUID dir5 = sceIoDopen("ms0:/PSP/GAME/CyanogenPSP/downloads");
	
	if (dirExists("ms0:/PSP/GAME/CyanogenPSP/downloads"))
	{
		sceIoDclose(dir5);
	}
	else 
	{
		sceIoMkdir("ms0:/PSP/GAME/CyanogenPSP/downloads",0777);
	}
	
	SceUID dir6 = sceIoDopen("ms0:/ISO");
	
	if (dirExists("ms0:/ISO"))
	{
		sceIoDclose(dir6);
	}
	else 
	{
		sceIoMkdir("ms0:/ISO",0777);
	}
}

void openOSK(char * message, char * initialMessage, int textLimit, int lang)
{
	int skip = 0;
	int Keyboard = 0;
	char data[20] = "";
	
	OSL_IMAGE *keyboardBg = oslLoadImageFilePNG(keyBoardBgPath, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);

	while(!osl_quit)
	{
	if (!skip)
	{
		oslStartDrawing();
		oslDrawImageXY(keyboardBg, 0, 0);
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
		battery(370,2,1);
		digitaltime(420,4,0,hrTime);
		if (Keyboard == 0)
		{
			oslInitOsk(message, initialMessage, textLimit, 1, lang);  
			Keyboard = 1; 
			oslDrawOsk(); 
			if (oslGetOskStatus() == PSP_UTILITY_DIALOG_NONE)
			{
				if (oslOskGetResult() == OSL_OSK_CANCEL) 
					Keyboard = 2; 
				else
				{
					oslOskGetText(data);
					sprintf(tempMessage, "%s", data);
					sprintf(tempPin, "%s", data);
					sprintf(tempData, "%s", data);
					Keyboard = 2; 
					oslDeleteImage(keyboardBg);
				}
				oslEndOsk();		
				return;
			}
			oslEndDrawing(); 
		}
		Keyboard = 0; 
		
		oslReadKeys();
		if (osl_pad.held.R && osl_keys->pressed.triangle)
		{
			screenshot();
		}
	}
	oslEndFrame();
    skip = oslSyncFrame();
}
}

int isEmpty(FILE *file) //Check to make sure its not a empty file
{
    long savedOffset = ftell(file);
    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0)
    {
        return 1;
    }

    fseek(file, savedOffset, SEEK_SET);
    return 0;
}

/*This was originally coded by Blade_punk. I modified it to add the X, and Y bits. */
void fadeOut(OSL_IMAGE* bg,int x, int y)//Name and params taken
{
	for(transp = 0; transp < 255; transp += 3)//Sets the transparency to 0 aka invisible. Then it increases by 3 until it equals 255 aka completely visible
	{
		oslStartDrawing();//Start drawing
		fade = RGBA(0, 0, 0, transp);//Set the colour of the rectangle. Sets the alpha to what transp currently is
		oslDrawImageXY(bg,x,y);//Draws an image in the background
		oslDrawFillRect(0, 0, 480, 272, fade);//Draws the rectangle over the screen
		oslEndDrawing();//Finish drawing
		oslEndFrame();//End the current frame
    	oslSyncFrame();//Sync everything
	}
}
void fadeIn(OSL_IMAGE* bg, int x, int y)//Name and params taken
{
	for(transp = 255; transp > 0; transp -= 3)//Sets the transparency to 255 aka completely visible. Then it decreases by 3 until it equals 255 aka invisible
	{
		oslStartDrawing();//Start drawing
		fade = RGBA(0, 0, 0, transp);//Set the colour of the rectangle. Sets the alpha to what transp currently is
		oslDrawImageXY(bg,x,y);//Draws an image in the background
		oslDrawFillRect(0, 0, 480, 272, fade);//Draws the rectangle over the screen
		oslEndDrawing();//Finish drawing
		oslEndFrame();//End the current frame
    	oslSyncFrame();//Sync everything
	}
}

int disableUsb(void)
{
	if(usbStatus)
	{
		sceUsbDeactivate(0);
		pspUsbDeviceFinishDevice();
		sceUsbStop(PSP_USBSTOR_DRIVERNAME, 0, 0);
		sceUsbStop(PSP_USBBUS_DRIVERNAME, 0, 0);
		usbStatus = 0;
		usbStat = 0;
		sceKernelDelayThread(300000);
	}
	return 0;
}

int enableUsb()
{
	if (usbStatus == 1)
	{
		disableUsb();
		return 0;
	}

	if(!usbModuleStatus)
	{
		pspSdkLoadStartModule("flash0:/kd/semawm.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstor.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstormgr.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstorms.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstorboot.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbdevice.prx", PSP_MEMORY_PARTITION_KERNEL);
		usbModuleStatus = 1;
	}

	if (!usbStatus)
	{
		sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
		sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0);
		sceUsbstorBootSetCapacity(0x800000);
		sceUsbActivate(0x1c8);
		usbStatus = 1;
		usbStat = 1;
		sceKernelDelayThread(300000);
	}
	
	return 1;
}

int enableUsbEx(u32 device)
{
	if (usbStatus == 1)
	{
		disableUsb();
		return 0;
	}

	pspUsbDeviceSetDevice(device, 0, 0);
	
	if(!usbModuleStatus)
	{
		pspSdkLoadStartModule("flash0:/kd/usbdevice.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/semawm.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstor.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstormgr.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstorms.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbstorboot.prx", PSP_MEMORY_PARTITION_KERNEL);
		pspSdkLoadStartModule("flash0:/kd/usbdevice.prx", PSP_MEMORY_PARTITION_KERNEL);
		usbModuleStatus = 1;
	}
	
	if (!usbStatus)
	{
		sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
		sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0);
		sceUsbActivate(0x1c8);
		usbStatus = 1;
		usbStat = 1;
		sceKernelDelayThread(300000);
	}
	
	return 1;
}

int isUSBCableConnected(){
    return (sceUsbGetState() & PSP_USB_CABLE_CONNECTED);
}

char getPSPNickname()
{
	return sceUtilityGetSystemParamString(1, nickname, 25);
}
