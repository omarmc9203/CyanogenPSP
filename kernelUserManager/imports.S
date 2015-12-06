	.set noreorder

#include "pspstub.s"

    STUB_START "pspIplUpdate",0x40090000,0x00030005
	STUB_FUNC  0xAD923E2F,pspIplUpdateGetIpl
	STUB_END
	
	STUB_START "SystemCtrlForUser",0x40090000,0x00020005
	STUB_FUNC  0x1090A2E1,sctrlHENGetVersion
	STUB_FUNC  0x5328B431,sctrlHENGetMinorVersion
    STUB_END
