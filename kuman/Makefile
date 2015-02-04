all:
	psp-packer kuman.prx

TARGET = kuman
OBJS = main.o imports.o

CFLAGS = -Os -G0
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

USE_KERNEL_LIBC = 1
USE_KERNEL_LIBS = 1

LDFLAGS = -mno-crt0 -nostartfiles
LIBS = -lpspident -lpsppower -lpspwlan -lpsprtc -lpspnand_driver

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
