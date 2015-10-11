	.set noreorder

#include "pspstub.s"

	STUB_START "myLib",0x40090000,0x00020005
	STUB_FUNC  0x19F94895,getBrightness
	STUB_FUNC  0xB6551455,setBrightness
	STUB_FUNC  0xAC18FDEC,displayEnable
	STUB_FUNC  0xDA2E4C8C,displayDisable
	STUB_END
