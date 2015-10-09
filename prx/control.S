   .set noreorder 

#include "pspstub.s" 

   STUB_START "buttons",0x10090000,0x00010005 
   STUB_FUNC  0xE2D9AF83,getbuttons 
   STUB_END 
