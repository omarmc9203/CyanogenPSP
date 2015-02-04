	.set noreorder

#include "pspstub.s"

	STUB_START "supportlib",0x40090000,0x00150005
	STUB_FUNC  0x19F94895,getBrightness
	STUB_FUNC  0xB6551455,setBrightness
	STUB_FUNC  0x1191DE7B,imposeSetBrightness
	STUB_FUNC  0x75A623EA,imposeGetBrightness
	STUB_FUNC  0xAC18FDEC,displayEnable
	STUB_FUNC  0xDA2E4C8C,displayDisable
	STUB_FUNC  0x521EAC81,imposeGetVolume
	STUB_FUNC  0x0F4D9C96,imposeSetVolume
	STUB_FUNC  0x4F0D8ADE,readButtons
	STUB_FUNC  0x98B8D7B4,MEEnable
	STUB_FUNC  0x0D340F77,MEDisable
	STUB_FUNC  0xBF6F954F,imposeGetMute
	STUB_FUNC  0x0CBCC445,imposeSetMute
	STUB_FUNC  0xC69E44DE,imposeSetHomePopup
	STUB_FUNC  0x4F113DD9,imposeGetEqualizer
	STUB_FUNC  0xB767B17B,imposeSetEqualizer
	STUB_FUNC  0x21B02AF5,getModelKernel
	STUB_FUNC  0x8AD688A8,setKernelBusClock
	STUB_FUNC  0x54670E8A,setKernelCpuClock
	STUB_FUNC  0x57AAF69E,getModuleListID
	STUB_FUNC  0xC5985F94,stopUnloadModule
	STUB_END
