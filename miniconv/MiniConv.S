	.set noreorder

#include "pspstub.s"

	STUB_START "MiniConv",0x00090000,0x000D0005
	STUB_FUNC  0x8CB88CB3,miniConvUTF16LEConv
	STUB_FUNC  0x68D7BA0A,miniConvUTF16BEConv
	STUB_FUNC  0xC1004BB4,miniConvGetConvCount
	STUB_FUNC  0xDADC4B59,miniConvGetConvCharset
	STUB_FUNC  0x7D322AF7,miniConvSetFileSystemConv
	STUB_FUNC  0xE4AF72B4,miniConvHaveFileSystemConv
	STUB_FUNC  0xBE3C38BC,miniConvFileSystemConv
	STUB_FUNC  0x3121FBB6,miniConvSetDefaultSubtitleConv
	STUB_FUNC  0x05DC7AFF,miniConvHaveDefaultSubtitleConv
	STUB_FUNC  0xDEE9CF5A,miniConvHaveSubtitleConv
	STUB_FUNC  0x59BC155A,miniConvDefaultSubtitleConv
	STUB_FUNC  0xA898AABE,miniConvSubtitleConv
	STUB_FUNC  0x40EBBB6B,miniConvFreeMemory
	STUB_END
