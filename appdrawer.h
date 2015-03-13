#include <pspkernel.h>
#include <oslib/oslib.h>
#include <pspumd.h> 
#include <string.h>
#include <psploadexec.h> 
#include <psploadexec_kernel.h> 

OSL_IMAGE *ic_launcher_calculator, *ic_launcher_clock, *ic_launcher_umd, *ic_launcher_gallery, *ic_launcher_filemanager, *ic_launcher_game, *backdrop;

void appdrawer_loadImages();
int appdrawer();
