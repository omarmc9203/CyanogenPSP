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

#ifndef  __LIB_MINI_CONV_H__
#define __LIB_MINI_CONV_H__

#ifdef __cplusplus
extern "C" {
#endif

char* miniConvUTF16LEConv(const unsigned short* utf16le);
char* miniConvUTF16BEConv(const unsigned short* utf16be);
int miniConvGetConvCount();
char* miniConvGetConvCharset(int index);
void miniConvSetFileSystemConv(const char* charset);
int miniConvHaveFileSystemConv();
char* miniConvFileSystemConv(const unsigned char* s);
void miniConvSetDefaultSubtitleConv(const char* charset);
int miniConvHaveDefaultSubtitleConv();
int miniConvHaveSubtitleConv(const char* charset);
char* miniConvDefaultSubtitleConv(const unsigned char* s);
char* miniConvSubtitleConv(const unsigned char* s, const char* charset);
void miniConvFreeMemory(void* mem);
#ifdef __cplusplus
}
#endif

#endif
