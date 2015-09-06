TARGET = CyanogenPSP
OBJS = main.o appdrawer.o home.o calculator.o lock.o settingsmenu.o clock.o recoverymenu.o imports.o multi.o power_menu.o \
	   apollo.o fm.o game.o mp3player.o messenger.o screenshot.o gallery.o language.o \
	   include/utils.o include/pgeZip.o include/ram.o include/common.o \
	   prx/display.o prx/sound.o \
	   #scepower.o
	   
BUILD_PRX = 1
PSP_LARGE_MEMORY = 1

VERSION	= 5.2

CFLAGS = -O2 -g -G0 -Wall -Werror
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =

MYLIBS=
STDLIBS= -losl -lpng -lz -lpspsystemctrl_user \
         -lpsphprm -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspwlan \
		 -lpsputility -lpsphttp -lpspssl -lpspgum -lpspgu -lm \
         -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -ljpeg \
		 -lpspvram -lpspkubridge -lpspreg \
		 -lpspusb -lpspusbstor -lpspusbdevice -lpspmp3 -lmad -lpspsystemctrl_kernel -lpspvshctrl -lpsprtc_driver -lpspreg_driver \
		  -lpspaudiolib -lpspaudio -lpspaudiocodec\
		 
LIBS=$(STDLIBS) $(MYLIBS)

LDFLAGS =
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = CyanogenPSP Stable v$(VERSION)
PSP_EBOOT_ICON = ICON0.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak