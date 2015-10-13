   .set noreorder 

#include "pspstub.s" 

   STUB_START "module",0x40090000,0x000A0005 
   STUB_FUNC  0x57AAF69E,getModuleListID 
   STUB_FUNC  0xCDC32C29,startLoadModule 
   STUB_FUNC  0xC5985F94,stopUnloadModule 
   STUB_END 
