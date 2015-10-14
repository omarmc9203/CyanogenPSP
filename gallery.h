#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <pspaudio.h>
#include <pspaudiolib.h>
#include <psppower.h>
#include <pspiofilemgr.h>
#include <oslib/oslib.h>

#define MAX_GALLERY_DISPLAY			3 // max amount of files displayed on-screen.
#define GALLERY_DISPLAY_X			78 // X value of where the filebrowser is displayed.
#define GALLERY_DISPLAY_Y			82 // Y value of the filebrowser is displayed.
#define GALLERY_CURR_DISPLAY_Y     	62 
#define THUMBNAIL_DISPLAY_Y      	65 

#define galley_xSelection 0 //The distance between the X axis of the two selections
#define galley_ySelection 56 //The distance between the Y axis of the two selections

OSL_IMAGE *gallerybg, *gallerySelection, *galleryThumbnail, *galleryBar, *wallpaper;

void galleryUp();
void galleryDown();
void galleryUpx5();
void galleryDownx5();
void galleryDisplay();
int changeWallpaper();
int showImage(char * path, int n);
void galleryControls();
char * galleryBrowse(const char * path);
void galleryUnload();
int galleryView(char * browseDirectory);
int galleryApp();
