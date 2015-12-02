INCLUDE_DIR := include
OSLIB_DIR := $(INCLUDE_DIR)/oslib

TARGET = CyanogenPSP
OBJS = main.o appDrawer.o homeMenu.o calculator.o lockScreen.o settingsMenu.o clock.o recoveryMenu.o imports.o recentsMenu.o \
	   powerMenu.o musicPlayer.o fileManager.o gameLauncher.o mp3Lib.o messenger.o screenshot.o gallery.o language.o \
	   include/utils.o include/pgeZip.o include/ram.o include/common.o \
	   prx/display.o prx/imposeDriver.o prx/control.o prx/power.o \
	   #scepower.o
	   
BUILD_PRX = 1
PSP_LARGE_MEMORY = 1

VERSION	= 5.2

CFLAGS = -O2 -g -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
INCDIR = $(INCLUDE_DIR) \
		 $(OSLIB_DIR)
		
LIBDIR = libs

LIBS = -losl -lpng -lz -lpspsystemctrl_user \
         -lpsphprm -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspwlan \
		 -lpsputility -lpsphttp -lpspssl -lpspgum -lpspgu -lm \
         -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -ljpeg \
		 -lpspvram -lpspkubridge -lpspreg \
		 -lpspusb -lpspusbstor -lpspusbdevice -lpspmp3 -lmad -lpspsystemctrl_kernel -lpspvshctrl -lpsprtc_driver -lpspreg_driver \
		 -lpspaudiolib -lpspaudio -lpspaudiocodec

LDFLAGS =
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = CyanogenPSP Stable v$(VERSION)
PSP_EBOOT_ICON = ICON0.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak