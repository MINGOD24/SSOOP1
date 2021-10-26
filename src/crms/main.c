#include "functions.h"
#include <stdio.h>  
#include <stdlib.h>
#include <string.h> 

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
    (byte & 0x80 ? '1' : '0'),     \
        (byte & 0x40 ? '1' : '0'), \
        (byte & 0x20 ? '1' : '0'), \
        (byte & 0x10 ? '1' : '0'), \
        (byte & 0x08 ? '1' : '0'), \
        (byte & 0x04 ? '1' : '0'), \
        (byte & 0x02 ? '1' : '0'), \
        (byte & 0x01 ? '1' : '0')

extern char* VIRTUAL_MEMORY_PATH;
extern FILE *fptr;
extern PCBTable* PCB_TABLE;
extern ErrorCode CR_ERROR;

int main(int argc, char **argv) 
{
    // AGREGAR 'NO_ERROR' COMO PRIMER CASO DEL ENUM DE ERRORES.

    // -------------------------------- VALID MOUNT ----------------------------------

    // cr_mount(argv[1]);
    // if (CR_ERROR) {
    //     cr_strerror(CR_ERROR);
    //     exit(1);
    // }
    // printf("%s\n", VIRTUAL_MEMORY_PATH);

    // -------------------------------- CR_MOUNT ERROR --------------------------------

    // cr_mount(argv[1]);
    // if (CR_ERROR) {
    //     cr_strerror(CR_ERROR);
    //     exit(1);
    // }

    // -------------------------------- START_PROCESS & LS_PROCESSES - CREATE ---------------------------

    // cr_mount(argv[1]);
    // cr_ls_processes();
    // cr_start_process(44, "TEST");

    // -------------------------------- START_PROCESS & LS_PROCESSES -  SHOW CREATED ---------------------------

    // cr_mount(argv[1]);
    // cr_ls_processes();

    // -------------------------------- START_PROCESS - CREATE REPEATED ID ERROR ---------------------------

    // cr_mount(argv[1]);
    // cr_start_process(44, "TEST");
    // if (CR_ERROR) {
    //     cr_strerror(CR_ERROR);
    //     exit(1);
    // }

    // -------------------------------- FINISH_PROCESS - DELETE ---------------------------

    // cr_mount(argv[1]);
    // cr_ls_processes();
    // cr_finish_process(44);

    // --------------------------------  FINISH_PROCESS -  SHOW DELETED ---------------------------

    // cr_mount(argv[1]);
    // cr_ls_processes();

    // -------------------------------- FINISH_PROCESS - DELETE MISSING ID ERROR ---------------------------

    // cr_mount(argv[1]);
    // cr_finish_process(44);
    // if (CR_ERROR) {
    //     cr_strerror(CR_ERROR);
    //     exit(1);
    // }

    // -------------------------------- LS_FILES & CR_EXISTS  ---------------------------

    // cr_mount(argv[1]);
    // cr_ls_files(9);
    // printf("%i\n", cr_exists(9, "day.png"));
    // printf("%i\n", cr_exists(9, "theme.wav"));

    // printf("\n%i\n", cr_exists(9, "hola.txt"));

    // -------------------------------- LS_FILES - INVALID ID ERROR ---------------------------

    // cr_mount(argv[1]);
    // cr_ls_files(44);
    // if (CR_ERROR) {
    //     cr_strerror(CR_ERROR);
    //     exit(1);    
    // }

    // -------------------------------- CR_EXISTS - INVALID ID ERROR ---------------------------

    // cr_mount(argv[1]);
    // cr_exists(44, "TEST.txt");
    // if (CR_ERROR) {
    //     cr_strerror(CR_ERROR);
    //     exit(1);    
    // }

    // -------------------------------- CR_OPEN - READ MODE - INVALID FILE ERROR ---------------------------

    // cr_mount(argv[1]);
    // cr_open(9, "TEST.txt", 'r');
    // if (CR_ERROR) {
    //     cr_strerror(CR_ERROR);
    //     exit(1);    
    // }

    // -------------------------------- CR_OPEN - WRITE MODE - CREATE FILE ---------------------------

    // cr_mount(argv[1]);
    // cr_ls_files(9);
    // cr_open(9, "TEST.txt", 'w');
    // printf("\n---------\n");
    // cr_ls_files(9);

    // -------------------------------- CR_OPEN - WRITE MODE - SHOW FILE ---------------------------

    // cr_mount(argv[1]);

    // CrmsFile* file = cr_open(9, "TEST.txt", 'r');
    // printf("FileName 'r' mode: %s\n", file->fileName);

    // file = cr_open(9, "TEST.txt", 'w');
    // printf("FileName 'w' mode: %s\n", file->fileName);

    // -------------------------------- CR_OPEN - BOTH MODES - INVALID ID ERROR ---------------------------

    // cr_mount(argv[1]);

    // cr_open(44, "TEST.txt", 'r');
    // if (CR_ERROR) {
    //     cr_strerror(CR_ERROR);
    // }

    // printf("\n---------\n");

    // cr_open(44, "TEST.txt", 'w');
    // if (CR_ERROR) {
    //     cr_strerror(CR_ERROR);
    //     exit(1);    
    // }

    // -------------------------------- CR_OPEN -  INVALID MODE ERROR ---------------------------

    // cr_mount(argv[1]);
    // cr_open(9, "TEST.txt", 'x');
    // if (CR_ERROR) {
    //     cr_strerror(CR_ERROR);
    //     exit(1);    
    // }

    // -------------------------------- CR_DELETE  ---------------------------

    // cr_mount(argv[1]);

    // for (int i = 0; i < 16; i++)
    // {
    //     fseek(fptr, 4096 + i, SEEK_SET);
    //     unsigned char byte[1];
    //     fread(byte, 1, 1, fptr);
    //     printf("Byte: " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(byte[0]));
    // }
    // printf("\n---------\n");

    // CrmsFile* file = cr_open(27, "caramel.wav", 'r');
    // cr_delete_file(file);
    //     printf("\n\n");

    // for (int i = 0; i < 16; i++)
    // {
    //     fseek(fptr, 4096 + i, SEEK_SET);
    //     unsigned char byte[1];
    //     fread(byte, 1, 1, fptr);
    //     printf("Byte: " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(byte[0]));
    // }




    // -------------------------------- CR_READ FULL  ---------------------------

    // cr_mount(argv[1]);
    // CrmsFile* crmsFile = cr_open(27, "amogus.mp4", 'r');
    // FILE *fp = fopen("amogus.mp4", "wb");
    // printf("SIZE: %i\n", crmsFile->fileSize);
    // unsigned char buffer[10000];
    // int byteLeidosTotales = 0;
    // int byteLeidos = cr_read(crmsFile, buffer, 10000);
    // byteLeidosTotales += byteLeidos;
    // printf("\nBytes Leidos: %i\n", byteLeidos);
    // while (byteLeidos != 0)
    // {
    //     if (byteLeidosTotales == crmsFile->fileSize)
    //     {
    //         break;
    //     }
        
    //     fwrite(buffer, byteLeidos, 1, fp);
    //     byteLeidos = cr_read(crmsFile, buffer, 10000);
    //     byteLeidosTotales += byteLeidos;
    // }
    // fclose(fp);



    // -------------------------------- CR_CLOSE WITHOUT CLOSING FILE VALGRIND ---------------------------

    // cr_mount(argv[1]);
    // CrmsFile* crmsFile = cr_open(0, "secret.txt", 'r');
    // printf("File Name: %s", crmsFile->fileName);


   // -------------------------------- CR_CLOSE CLOSING FILE VALGRIND ---------------------------

    // cr_mount(argv[1]);
    // CrmsFile* crmsFile = cr_open(0, "secret.txt", 'r');
    // printf("File Name: %s", crmsFile->fileName);
    // cr_close(crmsFile);
    




    destroy_all(PCB_TABLE);
}

