#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

Status edit_content(TagInfo *tagInfo,char *option,char *new_value){

    //open original file in read+ mode
    tagInfo->fptr_mp3 = fopen("sample.mp3","r+b");
    if(tagInfo->fptr_mp3==NULL){
        printf("Error opening file\n");
        return e_failure;
    }
    //creating or openinf duplicate file in write mode
    tagInfo->dup_mp3 = fopen("duplicate.mp3","wb");
    if(tagInfo->dup_mp3==NULL){
        printf("Error opening duplicate file\n");
        fclose(tagInfo->fptr_mp3);
        return e_failure;
    }

    // calculating the total file size of original file
    fseek(tagInfo->fptr_mp3,0,SEEK_END);
    long size = ftell(tagInfo->fptr_mp3);
    rewind(tagInfo->fptr_mp3);

    char *buffer = (char *)malloc(size);
    if(buffer == NULL){
        printf("Memory allocation failed\n");
        fclose(tagInfo->fptr_mp3);
        fclose(tagInfo->dup_mp3);
        return e_failure;
    }

    //copying contents of original file to duplicate file
    if(fread(buffer,1,size,tagInfo->fptr_mp3)!=size){
        printf("Error reading from original file\n");
        free(buffer);
        fclose(tagInfo->fptr_mp3);
        fclose(tagInfo->dup_mp3);
        return e_failure;
    }
    if(fwrite(buffer,1,size,tagInfo->dup_mp3)!=size){
        printf("Error writing to duplicate file\n");
        free(buffer);
        fclose(tagInfo->fptr_mp3);
        fclose(tagInfo->dup_mp3);
        return e_failure;
    }
    free(buffer);
    fclose(tagInfo->dup_mp3);

    tagInfo->dup_mp3 = fopen("duplicate.mp3","r+b");
    if(tagInfo->dup_mp3==NULL){
        printf("Error opening duplicate file\n");
        fclose(tagInfo->dup_mp3);
        return e_failure;
    }

    char frame_id[5];
    int max_len = 0;
    if(strcmp(option,"-t")==0){
        strcpy(frame_id,"TIT2");//title
        max_len = sizeof(tagInfo->title) - 1;
    }
    else if(strcmp(option,"-a")==0){
        strcpy(frame_id,"TPE1");//artist
        max_len = sizeof(tagInfo->artist) - 1;
    }
    else if(strcmp(option,"-A")==0){
        strcpy(frame_id,"TALB");//album
        max_len = sizeof(tagInfo->album) - 1;
    }
    else if(strcmp(option,"-y")==0){
        strcpy(frame_id,"TYER");//year
        max_len = sizeof(tagInfo->year) - 1;
    }
    else if(strcmp(option,"-m")==0){
        strcpy(frame_id,"TCON");//genre
        max_len = 1; // genre is a byte
    }
    else if(strcmp(option,"-c")==0){
        strcpy(frame_id,"COMM");//comments
        max_len = sizeof(tagInfo->comment) - 1;
    }
    else{
        printf("Invalid option\n");
        fclose(tagInfo->fptr_mp3);
        fclose(tagInfo->dup_mp3);
        return e_failure;
    }

   // printf("Max allowed length for this field: %d characters\n", max_len);

    if (strlen(new_value)+1 > max_len) {
        printf("New value exceeds allowed size (%d bytes)\n", max_len);
        fclose(tagInfo->fptr_mp3);
        fclose(tagInfo->dup_mp3);
        return e_failure;
    }

    //read from the file and identify the id and size 
    fseek(tagInfo->dup_mp3,10,SEEK_SET);
    int i;
    printf("-----------------------------UPDATED DETAILS-----------------------------\n");
    printf("\n------------------------------------------------------------------------------\n");
    printf("                    MP3 TAG READER AND EDITOR FOR ID3v2                        \n");
    printf("------------------------------------------------------------------------------\n");
    for(i=0;i<6;i++){
        unsigned char id[5];
        if(fread(id,1,4,tagInfo->dup_mp3)!=4){
            fclose(tagInfo->dup_mp3);
            return e_failure;
        }
        id[4]='\0';
        //size conversion;
        unsigned char dup_size[4];
        if(fread(dup_size,1,4,tagInfo->dup_mp3)!=4){
            printf("Error reading frame size\n");
            fclose(tagInfo->dup_mp3);
            return e_failure;
        }
        unsigned int dup_frame_size;
        dup_frame_size = (dup_size[0]<<24) | (dup_size[1]<<16) | (dup_size[2]<<8) | dup_size[3] ;

        //skip flags
        fseek(tagInfo->dup_mp3,2,SEEK_CUR);
        long current = ftell(tagInfo->dup_mp3);
        
        if (strcmp((char *)id,frame_id)==0){
            if (strlen(new_value) + 1 > dup_frame_size) {
                printf("Updated  value exceeds actual frame size (%u bytes)\n", dup_frame_size);
                fclose(tagInfo->fptr_mp3);
                fclose(tagInfo->dup_mp3);
                return e_failure;
            }
            fseek(tagInfo->dup_mp3,current+1,SEEK_SET);//current +1 to exclude the encoding byte
            fwrite(new_value,1,strlen(new_value),tagInfo->dup_mp3);

            int pad = dup_frame_size - 1- strlen(new_value);
            for(int j=0;j<pad;j++){
                fputc('\0',tagInfo->dup_mp3);
            }
        }
        fseek(tagInfo->dup_mp3,current,SEEK_SET);

        //display the frames

        char *dup_frame_data = (char *)malloc(dup_frame_size+1);
        if (dup_frame_data == NULL) {
            fclose(tagInfo->fptr_mp3);
            fclose(tagInfo->dup_mp3);
            return e_failure;
        }

        if(fread(dup_frame_data,1,dup_frame_size,tagInfo->dup_mp3)!=dup_frame_size){
            printf("Error reading frame data\n");
            free(dup_frame_data);
            fclose(tagInfo->fptr_mp3);
            fclose(tagInfo->dup_mp3);
            return e_failure;
        }
        dup_frame_data[dup_frame_size]='\0';
        if (strcmp((char *)id,"TIT2")==0){
            printf("%-15s: %s\n","Title",dup_frame_data+1);
        }
        else if (strcmp((char *)id,"TPE1")==0){
            printf("%-15s: %s\n","Artist",dup_frame_data+1);
        }
        else if (strcmp((char *)id,"TALB")==0){
            printf("%-15s: %s\n","Album",dup_frame_data+1);
        }
        else if (strcmp((char *)id,"TYER")==0){
            printf("%-15s: %s\n","Year",dup_frame_data+1);
        }
        else if (strcmp((char *)id,"TCON")==0){
            printf("%-15s: %s\n","Genre",dup_frame_data+1);
        }
        else if (strcmp((char *)id,"COMM")==0){
            printf("%-15s: %s\n","Comments",dup_frame_data+1);
        }
        else{
            return e_failure;
        }
        free(dup_frame_data);
    }
    fclose(tagInfo->dup_mp3);
    fclose(tagInfo->fptr_mp3);
    printf("------------------------------------------------------------------------------\n");
    printf("---------------------DETAILS DISPLAYED SUCCESSFULLY---------------------------\n\n");
    
    remove("sample.mp3");
    rename("duplicate.mp3","sample.mp3");
    return e_success;
}