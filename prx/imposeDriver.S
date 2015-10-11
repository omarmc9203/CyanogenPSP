	.set noreorder

#include "pspstub.s"

	STUB_START "imposeDriver",0x40090000,0x000A0005
	STUB_FUNC  0x521EAC81,imposeGetVolume
	STUB_FUNC  0x0F4D9C96,imposeSetVolume
	STUB_FUNC  0xBF6F954F,imposeGetMute
	STUB_FUNC  0x0CBCC445,imposeSetMute
	STUB_FUNC  0x4F113DD9,imposeGetEqualizer
	STUB_FUNC  0xB767B17B,imposeSetEqualizer
	STUB_FUNC  0xC69E44DE,imposeSetHomePopup
	STUB_FUNC  0x75A623EA,imposeGetBrightness
	STUB_FUNC  0x1191DE7B,imposeSetBrightness
	STUB_FUNC  0x43611234,imposeGetBacklightOffTime
	STUB_FUNC  0x36715C26,imposeSetBacklightOffTime
	STUB_END
