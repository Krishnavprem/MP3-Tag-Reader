/*
Name : Krishna V Prem
Date : 10-11-2025
Description : MP3 Tag Reader and Editor is a command-line tool that reads, displays, and modifies ID3 tag information in MP3 files. 
              It showcases concepts of file handling, metadata parsing, and data manipulation in C.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "types.h"
#include "main.h"

int main(int argc, char *argv[]){
    TagInfo tagInfo;
    memset(&tagInfo,0,sizeof(TagInfo));
    if (argc < 2){
        //printf("Insufficient arguments\n");
        display_help();
        return e_failure;
    }
    
    if (check_operation_type(argc,argv)==mp3_help){
        help_menu();
        return e_success;
    }
    else if (check_operation_type(argc,argv)==mp3_view){
        if(argc<3){
            display_help();
            return e_failure;
        }
        if (argv[2]==NULL){
            //printf("Insufficient arguments\n");
            display_help();
            return e_failure;
        }
        if (strstr(argv[2],".mp3")==NULL){
            printf(".mp3 file not present\n");
            return e_failure;
        }
        else{
            printf("You have choosed to view contents\n");
            
            if(read_content(&tagInfo)==e_success){
                return e_success;
            }
            else{
                printf("Couldn't view contents\n");
                return e_failure;
            }
        }
    }
    else if (check_operation_type(argc,argv)==mp3_edit){
        if(argc<5){
            printf("Insufficient arguments\n");
            display_help();
            return e_failure;
        }
        if(strstr(argv[4],".mp3")==0){
            printf(".mp3 file not present");
            return e_failure;
        }
        else{
            printf("You have choosed to edit contents\n");
            if (!(strcmp(argv[2],"-t")==0 || strcmp(argv[2],"-a")==0 || strcmp(argv[2],"-A")==0 ||
            strcmp(argv[2],"-m")==0 || strcmp(argv[2],"-y")==0 ||strcmp(argv[2],"-c")==0)){
                printf("Incorrect arguments\n");
                display_help();
                return e_failure;
            }
            else {
                if(edit_content(&tagInfo,argv[2],argv[3])==e_success){
                    return e_success;
                }
                else{
                    printf("Couldn't view contents\n");
                    return e_failure;
                }
            }  
        }
        
    }
    else{
        printf("It is unsupporting\n");
        return e_failure;
    }
}

Operationtype check_operation_type(int argc,char *argv[]){
    if(argc<2){
        return mp3_unsupported;
    }
    if (strcmp(argv[1],"-v")==0){
        return mp3_view;
    }
    else if(strcmp(argv[1],"-e")==0){
        return mp3_edit;
    }
    else if(argc==2 && (strcmp(argv[1],"--h")==0)){
        return mp3_help;
    }
    else {
        return mp3_unsupported;
    }
}
