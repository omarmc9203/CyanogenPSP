/*
PMP Mod
Copyright (C) 2006 jonny

Homepage: http://jonny.leffe.dnsalias.com
E-mail:   jonny@leffe.dnsalias.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*
"opendir" c code (~3 lines of perl -> ~100 lines of c :s)
*/


#ifndef opendir_h
#define opendir_h

#include <sys/unistd.h>
#include <string.h>
#include <pspiofilemgr.h>

#include "mem64.h"

struct dirElement{
	char 		d_name[264];
	char 		longname[264];
	SceIoStat   d_stat;
};

struct opendir_struct{
    struct dirElement *directory_entry;
    unsigned int number_of_directory_entries;
};

void opendir_safe_constructor(struct opendir_struct *p);
void opendir_close(struct opendir_struct *p);
char *opendir_open(struct opendir_struct *p, const char *directory, const char *directoryShort, char extFilter[][5], int extNumber, int includeDirs);
void sortDirectory(struct opendir_struct *directory);
void getExtension(char *fileName, char *extension, int extMaxLength);
void getFileName(char *fileName, char *onlyName);
int directoryUp(char *dirName);

#endif
