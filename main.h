#ifndef MAIN_H
#define MAIN_H

#include "types.h"
#include<stdio.h>
#include<string.h>


typedef struct
{
    char title[100];
    char artist[100];
    char album[100];
    char year[10];
    char comment[100];
    unsigned char genre;
    FILE *fptr_mp3;
    FILE *dup_mp3;
    unsigned char version;
    unsigned char revision;
    unsigned char flag;
    unsigned int size;

}TagInfo;

/*Perform read operation*/
Status read_content(TagInfo *tagInfo);

/*for reading frame parsing*/
Status view_frames(TagInfo *tagInfo);

/*perform edit operation*/
Status edit_content(TagInfo *tagInfo,char *option,char *new_value);
#endif