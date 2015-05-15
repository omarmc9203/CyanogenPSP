#include "utils.h"
#include "../home.h"
#include "../clock.h"
#include "../screenshot.h"


int transp;//Fo transparency
int fade;//To hold the colour of the rectangle blitted

char usbStatus = 0;
char usbModuleStatus = 0;

int fileExists(const char* path)
{
	SceUID dir = sceIoOpen(path, PSP_O_RDONLY, 0777);
	if (dir >= 0){
		sceIoClose(dir);
		return 1;
	}
	else {
		return 0;
	}
}

int dirExists(const char* path)
{
	SceUID dir = sceIoDopen(path);
	if (dir >= 0){
		sceIoDclose(dir);
		return 1;
	}
	else {
		return 0;
	}
}

void deleteUpdateFile()
{
	if (fileExists("ms0:/PSP/GAME/CyanogenMod.zip"))
	{
		sceIoRemove("ms0:/PSP/GAME/CyanogenMod.zip");
	}
}

void makeDownloadDir()
{
	SceUID dir = sceIoDopen(downloadPath);
	
	if (dirExists(downloadPath))
	{
		sceIoDclose(dir);
	}
	else 
	{
		sceIoMkdir("ms0:/PSP/GAME/CyanogenMod/downloads",0777);
}
}

void makeMusicDir()
{
	SceUID dir = sceIoDopen(musicPath);
	
	if (dirExists(musicPath))
	{
		sceIoDclose(dir);
	}
	else 
	{
		sceIoMkdir("ms0:/MUSIC",0777);
}
}

void openOSK(char * message, char * initialMessage, int textLimit, int lang)
{
	int skip = 0;
	int Keyboard = 0;
	char data[20] = "";
	
	OSL_IMAGE *keyboardBg = oslLoadImageFilePNG("system/settings/keyboard.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);

	while(!osl_quit)
	{
	if (!skip)
	{
		oslStartDrawing();
		oslDrawImageXY(keyboardBg, 0, 0);
		oslIntraFontSetStyle(Roboto, 0.5f,WHITE,0,0);
		battery(370,2,1);
		digitaltime(420,4,0);
		if (Keyboard == 0)
		{
			oslInitOsk(message, initialMessage, textLimit, 1, lang);  
			Keyboard = 1; 
			oslDrawOsk(); 
			if (oslGetOskStatus() == PSP_UTILITY_DIALOG_NONE)
			{
				if (oslOskGetResult() == OSL_OSK_CANCEL) 
				{
					Keyboard = 2; 
					oslDeleteImage(keyboardBg);
					return;
				}
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

/*Simple File Encryption and Decryption
by caroundw5h 
*/

void encryptFile(FILE *file)
{ 
    rewind(file); 
    /*go through the file and change all characters to 1 charater after */ 
    int i; 
    while ( ( i = fgetc(file) )!=EOF )
	{ 
        i = i + 1; 
        fseek(file, -1, SEEK_CUR); 
        fputc(i,file); 
        //fflush(file); 
        /* fflush not needed if making a call to fseek. Either one is needed for  
        manipulating a stream with i/o and vice versa */ 
        fseek(file, 0, SEEK_CUR); 
    } 
} 

void decryptFile(FILE *file)
{ 
    rewind(file); 
    /*go through the file and change all characters back from 1 */ 
    rewind(file); 
    int i; 
    while ( ( i = fgetc(file) )!=EOF )
	{ 
        i = i - 1; 
        fseek(file, -1, SEEK_CUR); 
        fputc(i,file); 
        //fflush(file); 
       /* fflush not needed if making a call to fseek. Either one is needed for  
        manipulating a stream with i/o and vice versa */ 
        fseek(file, 0, SEEK_CUR); 
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
   pspUsbDeviceSetDevice(device, 0, 0);
   sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
   sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0);
   sceUsbActivate(0x1c8);
   usbStatus = 1;
   sceKernelDelayThread(300000);
   }
   return 1;
}

int isUSBCableConnected(){
    return (sceUsbGetState() & PSP_USB_CABLE_CONNECTED);
}
