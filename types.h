#ifndef TYPES_H
#define TYPES_H

typedef enum{
    mp3_view,
    mp3_edit, 
    mp3_help,
    mp3_unsupported
} Operationtype;

typedef enum{
    e_success,
    e_failure,
}Status;


void display_help();
void help_menu();
Operationtype check_operation_type(int argc, char *argv[]);
#endif