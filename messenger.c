#include "messenger.h"
#include "homeMenu.h"
#include "appDrawer.h"
#include "powerMenu.h"
#include "clock.h"
#include "lockScreen.h"
#include "recentsMenu.h"
#include "screenshot.h"
#include "settingsMenu.h"
#include "include/utils.h"

char message[500] = "";	

int initiateMessage()
{
    char* nickname = (char*)malloc(100);
    int skip = 0;

    oslSetFont(Roboto);
	oslIntraFontSetStyle(Roboto, fontSize, BLACK, 0, INTRAFONT_ALIGN_LEFT);
	
    while(!osl_quit)
    {
       if (!skip)
       {
               oslStartDrawing();
               if (oslIsWlanPowerOn())
               {
                   oslDrawString(10, 10, "Please Enter nickname By Pressing X (Client)...");
                   oslDrawString(10, 25, "Please Press O To Act As Server...");
                   if (oslOskIsActive()){
                    oslDrawOsk();
                    if (oslGetOskStatus() == PSP_UTILITY_DIALOG_NONE)
                    {
                        if (oslOskGetResult() == OSL_OSK_CANCEL)
                        {
                            nickname = (char*)"Client";
                        }   
                        else
                        {
                            oslOskGetText(nickname);
                        }
                        oslEndOsk();
                    }
               }
               else
               {
                   oslDrawString(10, 40, "Please turn on the wlan switch!");
               }
               oslEndDrawing();
           }
           oslEndFrame();
           skip = oslSyncFrame();
           oslReadKeys();
           if (osl_keys->released.cross && oslIsWlanPowerOn())
           {
               oslInitOsk((char*)"Please enter nickname!", (char*)"Client", 99, 1, -1);

           }
      }

    }

    sceKernelExitGame();
    return 0;
}

//Prints some info:
void printInfo()
{
	u8 *macAddress = oslAdhocGetMacAddress();
	oslDrawStringf(10, 40, "Current state: %s", oslAdhocGetState() == ADHOC_INIT ? "OK" : "KO");
	oslDrawStringf(10, 50, "Your MAC address: %02X:%02X:%02X:%02X:%02X:%02X", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
}

//The client is connected and can send data:
void clientConnected(struct remotePsp *aPsp)
{
    int skip = 0;
	char mess[100] = "Hello distant World !!!";

    while(!osl_quit){
	
        if (!skip){
            oslStartDrawing();
			printInfo();
			oslDrawStringf(10, 60, "Press O to send a message to %s", aPsp->name);
            oslDrawString(10, 70, "Press O to abort");

            oslEndDrawing();
        }
        oslEndFrame();
        skip = oslSyncFrame();

        oslReadKeys();
		if (osl_keys->released.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslQuit();
		}
		else if (osl_keys->released.circle)
		{
			oslAdhocSendData(aPsp, mess, strlen( mess));
		}
	}
}

//Connects to a psp:
void doClient()
{
    int skip = 0;
	int quit = 0;
	int i = 0;
	int current = 0;

	int init = oslAdhocInit("ULUS99999");
	if (init)
	{
		char message[100] = "";
		sprintf(message, "adhocInit error: %i", init);
		oslMessageBox(message, "Debug",  oslMake3Buttons(OSL_KEY_CROSS, OSL_MB_OK , 0, 0, 0, 0));
		return;
	}

    while(!osl_quit && !quit){
        if (!skip){
            oslStartDrawing();

			printInfo();
			if (oslAdhocGetRemotePspCount())
				oslDrawString(10, 60, "Press X to request a connection");
				oslDrawString(10, 70, "Press O to abort");
			for (i=0; i < oslAdhocGetRemotePspCount(); i++)
			{
				if (i == current)
				    oslIntraFontSetStyle(Roboto, 1.0, RGBA(100,100,100,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
				else
				    oslIntraFontSetStyle(Roboto, 1.0, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
				oslDrawString(10, 100 + 15 * i, oslAdhocGetPspByIndex(i)->name);
			}
            oslEndDrawing();
        }
        oslEndFrame();
        skip = oslSyncFrame();

		oslReadKeys();
		if (osl_keys->released.down)
		{
			if (++current >= oslAdhocGetRemotePspCount())
				current = 0;
		}
		else if (osl_keys->released.up)
		{
			if (--current < 0)
				current = oslAdhocGetRemotePspCount() - 1;
		}
		else if (osl_keys->released.cross && oslAdhocGetRemotePspCount())
		{
			//Request a connection:
			int ret = oslAdhocRequestConnection(oslAdhocGetPspByIndex(current), 30, NULL);
			if (ret == OSL_ADHOC_ACCEPTED || ret == OSL_ADHOC_ESTABLISHED)
			{
				clientConnected(oslAdhocGetPspByIndex(current));
			}
		}
		else if (osl_keys->released.circle)
		{
			quit = 1;
		}
	}
	oslAdhocTerm();
}


//The server accepted the connection and it's ready to receive data:
void serverConnected(struct remotePsp *aPsp)
{
    int skip = 0;
	char buffer[100] = "";
	int dataLength = 0;

    while(!osl_quit){
		if (dataLength <= 0)
			dataLength = oslAdhocReceiveData(aPsp, buffer, 100);

        if (!skip){
            oslStartDrawing();

			printInfo();

			if (dataLength <= 0)
			{
				oslDrawStringf(10, 40, "Waiting for data from %s", aPsp->name);
			}
			else
			{
				oslDrawStringf(10, 40, "Data received from %s:", aPsp->name);
				oslDrawStringf(10, 55, buffer);
				oslDrawString(10, 70, "Press O to receive more data");
			}
			oslDrawString(150, 250, "Press X to quit");
            oslEndDrawing();
        }
        oslEndFrame();
        skip = oslSyncFrame();

        oslReadKeys();
		if (osl_keys->released.cross)
		{
			oslQuit();
		}
		else if (osl_keys->released.circle && dataLength > 0)
		{
			dataLength = 0;
			memset(buffer, sizeof(buffer), 0);
		}
	}
}


//Waits for a connection from a psp:
void doServer()
{
    int skip = 0;
	int quit = 0;
	char message[100] = "";
	int dialog = OSL_DIALOG_NONE;
	int connected = 0;

	int init = oslAdhocInit("ULUS99999");
	if (init)
	{
		snprintf(message, sizeof(message), "adhocInit error: %i", init);
		oslMessageBox(message, "Debug",  oslMake3Buttons(OSL_KEY_CROSS, OSL_MB_OK , 0, 0, 0, 0));
		return;
	}

    while(!osl_quit && !quit){
		struct remotePsp *reqPsp = oslAdhocGetConnectionRequest();

        if (!skip){
            oslStartDrawing();

			printInfo();
			oslDrawString(10, 40, "Press O to abort");
			if (reqPsp == NULL)
			{
				oslDrawString(10, 100, "Waiting for a connection request...");
			}
			else
			{
				snprintf(message, sizeof(message), "Accept request from psp : %s", reqPsp->name);
				oslDrawString(10, 100, message);
				if (oslGetDialogType() == OSL_DIALOG_NONE)
					oslInitMessageDialog(message, 1);
			}

            dialog = oslGetDialogType();
            if (dialog){
                oslDrawDialog();
                if (oslGetDialogStatus() == PSP_UTILITY_DIALOG_NONE){
                    if (dialog == OSL_DIALOG_MESSAGE){
                        int button = oslGetDialogButtonPressed();
                        if (button == PSP_UTILITY_MSGDIALOG_RESULT_YES)
						{
							oslAdhocAcceptConnection(reqPsp);
							connected = 1;
						}
                        else if (button == PSP_UTILITY_MSGDIALOG_RESULT_NO)
						{
							oslAdhocRejectConnection(reqPsp);
						}
                    }
                    oslEndDialog();
                }
			}

            oslEndDrawing();
        }
        oslEndFrame();
        skip = oslSyncFrame();

		if (connected)
			serverConnected(reqPsp);

		oslReadKeys();
		if (dialog == OSL_DIALOG_NONE)
		{
			if (osl_keys->released.circle)
			{
				quit = 1;
			}
		}
	}
	oslAdhocTerm();
}

void newMessage()
{	
	new_message = oslLoadImageFilePNG("system/app/messenger/new_message.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!new_message)
		debugDisplay();
	
	while (!osl_quit)
	{
		LowMemExit();
		
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));
		
		controls();	

		oslDrawImageXY(new_message, 0, 0);
		
		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
		}
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(new_message);
			messenger();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(messengerbg);
			messenger();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(messengerbg);
			home();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
		if (cursor->x >= 7 && cursor->x <= 435 && cursor->y >= 55 && cursor->y <= 86 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			doServer();
		}
		
		oslIntraFontSetStyle(Roboto, fontSize, BLACK, 0, INTRAFONT_ALIGN_LEFT);
		oslDrawString(14, 245, tempMessage);
		
		if (cursor->x >= 10 && cursor->x <= 362 && cursor->y >= 228 && cursor->y <= 270 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			openOSK("Enter Message", "", 128, -1);
		}
		
		if ((cursor->x >= 10 && cursor->x <= 405 && cursor->y >= 88 && cursor->y <= 118 && osl_keys->pressed.cross) && oslIsWlanPowerOn())
			doClient();
        else if ((cursor->x >= 410 && cursor->x <= 442 && cursor->y >= 92 && cursor->y <= 118 && osl_keys->pressed.cross) && oslIsWlanPowerOn())
			doServer();
		
        oslEndDrawing(); 
		oslEndFrame(); 
        oslSyncFrame();
	}
}

int messenger()
{	
	messengerbg = oslLoadImageFilePNG("system/app/messenger/messengerbg.png", OSL_IN_RAM, OSL_PF_8888);
	
	if (!messengerbg)
		debugDisplay();
	
	oslSetFont(Roboto);
		
	while (!osl_quit)
	{
		LowMemExit();

		oslStartDrawing();	
		
		oslClearScreen(RGB(0,0,0));
		
		oslIntraFontSetStyle(Roboto, fontSize, BLACK, 0, INTRAFONT_ALIGN_LEFT);
		
		controls();	

		oslDrawImageXY(messengerbg, 0, 0);

		navbarButtons(2);
		battery(330,2,0);
		digitaltime(381,4,0,hrTime);
		androidQuickSettings();
		volumeController();
		oslDrawImage(cursor);
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
		}
		
		if (osl_keys->pressed.circle)
		{
			oslDeleteImage(messengerbg);
			appdrawer();
		}
	
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 157 && cursor->y <= 213) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(messengerbg);
			appdrawer();
		}

		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 76 && cursor->y <= 155) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(messengerbg);
			home();
		}
		
		if ((cursor->x  >= 444 && cursor->x  <= 480) && (cursor->y >= 19 && cursor->y <= 75) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1);  
			multitask();
		}
		
		captureScreenshot();
		
		if (cursor->x >= 385 && cursor->x <= 428 && cursor->y >= 210 && cursor->y <= 258 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(messengerbg);
			newMessage();
		}
		
		/*
		if (cursor->x >= 378 && cursor->x <= 434 && cursor->y >= 20 && cursor->y <= 52 && osl_keys->pressed.cross)
		{
			oslPlaySound(KeypressStandard, 1);  
			oslDeleteImage(messengerbg);
			doClient();
		}
		*/
		
		oslEndDrawing(); 
		oslEndFrame(); 
		oslSyncFrame();
	}
	return 0;
}

