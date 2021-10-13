#include "functions.h"
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <stdlib.h>
#include <string.h> // strtok, strcpy, etc.


extern char* VIRTUAL_MEMORY_PATH;
extern FILE *fptr;
extern PCBTable* PCB_TABLE;

int main(int argc, char **argv) 
{
    cr_mount(argv[1]);
    cr_ls_processes();
    // int name = cr_exists(27, "grub.mp4");
    // printf("RETORNO: %i\n", name);
    // cr_ls_files(27);
    cr_start_process(34, "vicho pizarro");
    cr_ls_processes();
    cr_finish_process(34);
    cr_ls_processes();


}

