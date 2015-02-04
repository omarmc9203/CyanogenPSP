#ifndef __id3_h
#define __id3_h (1)

#include <stdio.h>

#define JPEG_IMAGE 1
#define PNG_IMAGE 2

struct ID3Tag {
    char   ID3Title[260];
    char   ID3Artist[260];
    char   ID3Album[260];
	char   ID3Year[12];
	char   ID3Comment[260];
	char   ID3GenreCode[12];
	char   ID3GenreText[260];
    char   versionfound[12];
    int    ID3Track;
    char   ID3TrackText[8];
    int    ID3EncapsulatedPictureType;
    int    ID3EncapsulatedPictureOffset; /* Offset address of an attached picture, NULL if no attached picture exists */
    int    ID3EncapsulatedPictureLength;
    int    ID3Length;
};

int ID3v2TagSize(const char *mp3path);
int ParseID3(char *mp3path, struct ID3Tag *target);

//Helper functions (used also by aa3mplayerME to read tags):
void readTagData(FILE *fp, int tagLength, char *tagValue);
int swapInt32BigToHost(int arg);
//short int swapInt16BigToHost(short int arg);

#endif
