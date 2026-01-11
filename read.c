#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "types.h"
#include "main.h"

Status read_content(TagInfo *tagInfo){

    //open files
    tagInfo->fptr_mp3 = fopen("sample.mp3","rb");//open file in binary mode
    if(tagInfo->fptr_mp3==NULL){
        printf("Cannot open file\n");
        return e_failure;
    }
    char tag[4];
    if(fread(tag,1,3,tagInfo->fptr_mp3)!=3){//read 3 bytes
        return e_failure;
    }
    tag[3]='\0';
    if(strcmp(tag,"ID3")!=0){
        printf("Error finding ID3 tag\n");
        fclose(tagInfo->fptr_mp3);
        return e_failure;
    }
    //read version and version revision
    unsigned char version[2];
    if(fread(version,1,2,tagInfo->fptr_mp3)!=2){
        fclose(tagInfo->fptr_mp3);
        return e_failure;
    }
    tagInfo->version =version[0];
    tagInfo->revision=version[1];

    //read flag
    unsigned char flag;
    if(fread(&flag,1,1,tagInfo->fptr_mp3)!=1){
        fclose(tagInfo->fptr_mp3);
        return e_failure;
    }
    tagInfo->flag=flag;

    //read size
    unsigned char size[4];
    if(fread(size,1,4,tagInfo->fptr_mp3)!=4){
        fclose(tagInfo->fptr_mp3);
        return e_failure;
    }

    /*tagsize is stored as synchsafe integers, ie in each byte msb is always 0
    ie it need to be encoded like*/
    tagInfo->size = (size[0] <<21) | (size[1]<<14) | (size[2]<<7) | size[3];
    //call frame parser
    view_frames(tagInfo);
    fclose(tagInfo->fptr_mp3);
    return e_success;
}

Status view_frames(TagInfo *tagInfo){
    fseek(tagInfo->fptr_mp3,10,SEEK_SET);
    int i;
    printf("-----------------------------SELECTED VIEW DETAILS-----------------------------\n");
    printf("\n------------------------------------------------------------------------------\n");
    printf("                    MP3 TAG READER AND EDITOR FOR ID3v2                        \n");
    printf("------------------------------------------------------------------------------\n");
    for(i=0;i<6;i++){
        unsigned char frame_id[5];
        if(fread(frame_id,1,4,tagInfo->fptr_mp3)!=4){
            fclose(tagInfo->fptr_mp3);
            return e_failure;
        }
        frame_id[4]='\0';

        unsigned char size[4];
        if(fread(size,1,4,tagInfo->fptr_mp3)!=4){
            fclose(tagInfo->fptr_mp3);
            return e_failure;
        }
        //covert the size from big endian to little endian
        unsigned int frame_size;
        frame_size = (size[0]<<24) | (size[1]<<16) | (size[2]<<8) | size[3] ;

        //skip the franeflags
        if(fseek(tagInfo->fptr_mp3,2,SEEK_CUR)!=0){
            //fclose(tagInfo->fptr_mp3);
            return e_failure;
        }
        //read the frame content

        char *frame_data = (char*)malloc(frame_size+1);
        if(frame_data == NULL){
            return e_failure;
        }
        if(fread(frame_data,1,frame_size,tagInfo->fptr_mp3)!=frame_size){
            free(frame_data);
            //fclose(tagInfo->fptr_mp3);
            return e_failure; 
        }
        frame_data[frame_size] = '\0';

        //display the frames
        if (strcmp((char *)frame_id,"TIT2")==0){
            printf("%-15s: %s\n","Title",frame_data+1);
        }
        else if (strcmp((char *)frame_id,"TPE1")==0){
            printf("%-15s: %s\n","Artist",frame_data+1);
        }
        else if (strcmp((char *)frame_id,"TALB")==0){
            printf("%-15s: %s\n","Album",frame_data+1);
        }
        else if (strcmp((char *)frame_id,"TYER")==0){
            printf("%-15s: %s\n","Year",frame_data+1);
        }
        else if (strcmp((char *)frame_id,"TCON")==0){
            printf("%-15s: %s\n","Genre",frame_data+1);
        }
        else if (strcmp((char *)frame_id,"COMM")==0){
            printf("%-15s: %s\n","Comments",frame_data+1);
        }
        else{
            return e_failure;
        }
        free(frame_data);
        //int pos=ftell(tagInfo->fptr_mp3);
    }
    printf("------------------------------------------------------------------------------\n");
    printf("---------------------DETAILS DISPLAYED SUCCESSFULLY---------------------------\n\n");
    return e_success;
}
