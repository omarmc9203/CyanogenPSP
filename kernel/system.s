	.set noreorder

#include "pspstub.s"

	STUB_START "system_extender",0x40090000,0x00060005
	STUB_FUNC  0xA94D367E,sysKernelPartitionMaxFreeMemSize
	STUB_END