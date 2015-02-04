/* 
 *	Copyright (C) 2008 cooleyes
 *	eyes.cooleyes@gmail.com 
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsdk.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "miniconv.h"


PSP_MODULE_INFO("MiniConv_library", 0, 1, 1);   // User Module
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(1024);

char* miniConvUTF16LEConv(const unsigned short* utf16le) {
	return utf16le_to_utf8(utf16le);
};

char* miniConvUTF16BEConv(const unsigned short* utf16be) {
	return utf16be_to_utf8(utf16be);
};

typedef struct  {
	const char* charset;
	utf8_convert_function convertor;
} convertor_t;

static convertor_t convertors[] = {
	{"UTF-8", NULL},
#ifdef GBK_CONV
	{"GBK", gbk_to_utf8},
#endif
#ifdef BIG5_CONV
	{"BIG5", big5_to_utf8},
#endif
#ifdef SJIS_CONV
	{"SHIFT-JIS", sjis_to_utf8},
#endif
#ifdef EUC_KR_CONV
	{"EUC-KR", euc_kr_to_utf8},
#endif
#ifdef MS_EE_CONV
	{"MS-EE", ms_ee_to_utf8},
#endif
#ifdef MS_CYRL_CONV
	{"MS-CYRL", ms_cyrl_to_utf8},
#endif
#ifdef ISO8859_1_CONV
	{"ISO-8859-1", iso8859_1_to_utf8},
#endif
#ifdef ISO8859_2_CONV
	{"ISO-8859-2", iso8859_2_to_utf8},
#endif
	{NULL, NULL}
};

static utf8_convert_function fs_convertor = NULL;
static utf8_convert_function sub_convertor = NULL;

int miniConvGetConvCount() {
	int count = 0;
	convertor_t *p = convertors;
	while( p->charset != NULL ) {
		count++;
		p++;
	}
	return count;
}

char* miniConvGetConvCharset(int index) {
	return convertors[index].charset;
}

void miniConvSetFileSystemConv(const char* charset) {
	convertor_t *p = convertors;
	while( p->charset != NULL ) {
		if (stricmp(charset, p->charset) == 0) {
			fs_convertor = p->convertor;
			return;
		}
		p++;
	}
	fs_convertor = NULL;
};

int miniConvHaveFileSystemConv() {
	return ( (fs_convertor == NULL) ? 0 : 1 );
};

char* miniConvFileSystemConv(const unsigned char* s) {
	return fs_convertor(s);
}

void miniConvSetDefaultSubtitleConv(const char* charset) {
	convertor_t *p = convertors;
	while( p->charset != NULL ) {
		if (stricmp(charset, p->charset) == 0) {
			sub_convertor = p->convertor;
			return;
		}
		p++;
	}
	sub_convertor = NULL;
};

int miniConvHaveDefaultSubtitleConv() {
	return ( (sub_convertor == NULL) ? 0 : 1 );
};

int miniConvHaveSubtitleConv(const char* charset) {
	convertor_t *p = convertors;
	while( p->charset != NULL ) {
		if (stricmp(charset, p->charset) == 0) {
			if ( p->convertor )
				return 1;
			else
				return 0;
		}
		p++;
	}
	return 0;
};

char* miniConvDefaultSubtitleConv(const unsigned char* s) {
	return sub_convertor(s);
};

char* miniConvSubtitleConv(const unsigned char* s, const char* charset) {
	convertor_t *p = convertors;
	while( p->charset != NULL ) {
		if (stricmp(charset, p->charset) == 0) {
			if ( p->convertor )
				return p->convertor(s);
			else
				return 0;
		}
		p++;
	}
	return 0;
};

void miniConvFreeMemory(void* mem) {
	free(mem);
}

int module_start(SceSize args, void *argp) {
	return 0;
}
