	.set noreorder

#include "pspimport.s"

	IMPORT_START "sceImpose_Driver",0x00010000
	IMPORT_FUNC	"sceImpose_Driver",0x531C9778,sceImposeGetParam 
	IMPORT_FUNC	"sceImpose_Driver",0x531C9778,sceImposeGetParam371

	IMPORT_FUNC	"sceImpose_Driver",0x810FB7FB,sceImposeSetParam
	IMPORT_FUNC	"sceImpose_Driver",0x810FB7FB,sceImposeSetParam371
	
