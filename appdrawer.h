#include <pspkernel.h>
#include <oslib/oslib.h>
#include <pspumd.h> 
#include <string.h>
#include <psploadexec.h> 
#include <psploadexec_kernel.h> 

OSL_IMAGE *background, *cursor, *music, *gmail, *messengericon, *browser, *google, *calc, *clockx, *email, *people, *calendar, *umd, 
		  *gallery, *fb, *settings,*pointer, *pointer1, *isoloadericon, *backicon, *homeicon, *multicon, *backdrop;
		  
OSL_FONT *Roboto;

void appdrawer_loadImages();
int appdrawer();
