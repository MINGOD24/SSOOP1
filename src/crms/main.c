#include "functions.h"
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <stdlib.h>
#include <string.h> // strtok, strcpy, etc.


extern char* VIRTUAL_MEMORY_PATH;
extern FILE *fptr;
extern PCBTable* PCB_TABLE;
ErrorCode CR_ERROR;

int main(int argc, char **argv) 
{
    cr_mount(argv[1]);
    // cr_ls_processes();
    // int name = cr_exists(27, "grub.mp4");
    // printf("RETORNO: %i\n", name);
    // cr_ls_files(27);
    // cr_start_process(34, "vicho pizarro");
    // cr_ls_processes();
    // cr_finish_process(34);
    // cr_ls_processes();
    // CrmsFile* crmsFile = cr_open(27, "juanito.mp3", 'w');
    // printf("File Name: %s\n", crmsFile->fileName);
    // int name = cr_exists(27, "juanito.mp3");
    // printf("RETORNO: %i\n", name);
    CrmsFile* crmsFile = cr_open(0, "secret.txt", 'r');
    FILE *fp = fopen("secret.txt", "wb");
    printf("SIZE: %i\n", crmsFile->fileSize);
    unsigned char buffer[100];
    int byteLeidos = cr_read(crmsFile, buffer, 100);
    printf("Bytes Leidos: %i\n", byteLeidos);
    while (byteLeidos != 0)
    {
        fwrite(buffer, byteLeidos, 1, fp);
        byteLeidos = cr_read(crmsFile, buffer, 100);
    }
    fclose(fp);
}

