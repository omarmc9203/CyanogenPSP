#include <pspkernel.h>
#include <oslib/oslib.h>
#include <pspumd.h> 
#include <string.h>
#include <psploadexec.h> 
#include <psploadexec_kernel.h> 

OSL_IMAGE *calc, *clockx, *umd, *gallery, *fb, *isoloadericon, *backdrop;
		  
OSL_FONT *Roboto;

void appdrawer_loadImages();
int appdrawer();
