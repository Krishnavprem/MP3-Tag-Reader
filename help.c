#include<stdio.h>
#include "types.h"

void display_help(){
    printf("USAGE: \n");
    printf("To view please pass like: ./a.out -v mp3filename\n");
    printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
    printf("To get help pass like: ./a.out --help\n");

}

void help_menu(){
    printf("\n----------------------HELP MENU----------------------\n");
    printf("\n1. -v -> to view mp3 file contents\n");
    printf("2. -e -> to edit mp3 file contents\n");
    printf("\t2.1 -t -> to edit song title\n");
    printf("\t2.2 -a -> to edit artist name\n");
    printf("\t2.3 -A -> to edit album name\n");
    printf("\t2.4 -y -> to edit year\n");
    printf("\t2.5 -m -> to edit content\n");
    printf("\t2.6 -c -> to edit comment \n");
    printf("-----------------------------------------------------\n");
}