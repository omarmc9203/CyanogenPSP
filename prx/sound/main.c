//    LightMP3
//    Copyright (C) 2007 Sakya
//    sakya_tg@yahoo.it
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#include <pspsdk.h>
#include <pspkernel.h>
#include <pspimpose_driver.h>
#include <pspsysreg.h>
#include <pspsysmem_kernel.h>

PSP_MODULE_INFO("soundlib", 0x1006, 1, 1);
PSP_MAIN_THREAD_ATTR(0);

u32 k1;

int imposeGetVolume(){
    k1 = pspSdkSetK1(0);
    int ret = sceImposeGetParam(PSP_IMPOSE_MAIN_VOLUME);
    pspSdkSetK1(k1);
    return ret;
}

int imposeSetVolume(int value){
    k1 = pspSdkSetK1(0);
    int ret = sceImposeSetParam(PSP_IMPOSE_MAIN_VOLUME, value);
    pspSdkSetK1(k1);
    return ret;
}

int imposeGetMute(){
    k1 = pspSdkSetK1(0);
    int ret = sceImposeGetParam(PSP_IMPOSE_MUTE);
    pspSdkSetK1(k1);
    return ret;
}

int imposeSetMute(int value){
    k1 = pspSdkSetK1(0);
    int ret = sceImposeSetParam(PSP_IMPOSE_MUTE, value);
    pspSdkSetK1(k1);
    return ret;
}

int imposeGetEqualizer(){
    k1 = pspSdkSetK1(0);
    int ret = sceImposeGetParam(PSP_IMPOSE_EQUALIZER_MODE);
    pspSdkSetK1(k1);
    return ret;
}

int imposeSetEqualizer(int value){
    k1 = pspSdkSetK1(0);
    int ret = sceImposeSetParam(PSP_IMPOSE_EQUALIZER_MODE, value);
    pspSdkSetK1(k1);
    return ret;
}

int module_start(SceSize args, void *argp){
	return 0;
}

int module_stop(){
	return 0;
}
