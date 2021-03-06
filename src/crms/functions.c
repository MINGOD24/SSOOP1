#include <unistd.h> // Symbolic Constants - access
#include "functions.h"
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <stdlib.h>
#include "structs.h"
#include<string.h>  
#include <math.h>

char* VIRTUAL_MEMORY_PATH;
FILE *fptr;
PCBTable* PCB_TABLE;

ErrorCode CR_ERROR;

// general functions
void cr_mount(char* memory_path)
{
    if (access(memory_path, F_OK) == 0)
    {
        VIRTUAL_MEMORY_PATH = memory_path;
        fptr = fopen(VIRTUAL_MEMORY_PATH, "r+b");
        PCB_TABLE = PCBTableInit();
    }
    else
    {
        //Manage error invalid path
        CR_ERROR = INVALID_MEM_PATH;
    }

}


void cr_ls_processes()
{
    for (int i = 0; i < 16; i++)
    {
        
        if (PCB_TABLE->entriesArray[i]->state)
        {
            printf("PID: %i, PROCESS_NAME: %s, PROCESS_STATE: %i\n", PCB_TABLE->entriesArray[i]->processId, PCB_TABLE->entriesArray[i]->processName, PCB_TABLE->entriesArray[i]->state);
        }
        
        
    }
    
}

int cr_exists(int process_id, char* file_name)
{
    for (int i = 0; i < 16; i++)
    {
        
        if (PCB_TABLE->entriesArray[i]->processId == process_id)
        {
            for (int j = 0; j < 10; j++)
            {
                if (strcmp((const char *)PCB_TABLE->entriesArray[i]->subEntriesArray[j]->fileName, file_name) == 0){
                    return 1;
                }
            }
            return 0;
            
        }
        
        
    }
    CR_ERROR = INVALID_PROCESSID_EXIST;
    return 0;
}


void cr_ls_files(int process_id)
{
    int found = 0;
    for (int i = 0; i < 16; i++)
    {
        
        if (PCB_TABLE->entriesArray[i]->processId == process_id)
        {
            found ++;
            for (int j = 0; j < 10; j++)
            {
                if (PCB_TABLE->entriesArray[i]->subEntriesArray[j]->valid)
                {
                    printf("%s\n", PCB_TABLE->entriesArray[i]->subEntriesArray[j]->fileName);
                }
            }
            
        }
        
        
    }
    if (found == 0)
    {
        CR_ERROR = INVALID_PROCESSID_LS;
    }
    
}

void cr_start_process(int process_id, char* process_name)
{
    int found = 0;
    for (int i = 0; i < 16; i++)
    {
        if (PCB_TABLE->entriesArray[i]->processId == process_id && PCB_TABLE->entriesArray[i]->state == 1)
        {
            CR_ERROR = PROCESSID_ALREADY_EXIST;
            break;
        }
        
        if (!PCB_TABLE->entriesArray[i]->state)
        {
            found ++;
            memcpy(PCB_TABLE->entriesArray[i]->processName, process_name, 12);
            PCB_TABLE->entriesArray[i]->processId = process_id;
            PCB_TABLE->entriesArray[i]->state = 1;

            unsigned char byteState = (unsigned char)1;

            fseek(fptr, 256 * i, SEEK_SET);
            fwrite(&byteState, 1, 1, fptr);

            unsigned char byteId = (unsigned char)process_id;
            fseek(fptr, 256 * i + 1, SEEK_SET);
            fwrite(&byteId, 1, 1, fptr);

            // unsigned char byteProcessName[12] = (unsigned char)process_name;
            fseek(fptr, 256 * i + 2, SEEK_SET);
            fwrite(process_name, 1, 12, fptr);
            break;
        }
        
    }
    if (!found && CR_ERROR != PROCESSID_ALREADY_EXIST)
    {
        CR_ERROR = PCB_FULL_START;
    }
    
    
    
}

//TODO: Falta liberar frames
void cr_finish_process(int process_id)
{
    int foundProcess = 0;
    for (int i = 0; i < 16; i++)
    {
        
        if (PCB_TABLE->entriesArray[i]->processId == process_id)
        {
            for (int j = 0; j < 10; j++)
            {
                PCB_TABLE->entriesArray[i]->subEntriesArray[j]->valid = 0;

                unsigned char byteValid = (unsigned char)0;
                fseek(fptr, 256 * i + 21 * j + 14, SEEK_SET);
                fwrite(&byteValid, 1, 1, fptr);
            }
            
            PCB_TABLE->entriesArray[i]->state = 0;
            PCB_TABLE->entriesArray[i]->processId = 0;
            
            unsigned char byteState = (unsigned char)0;
            fseek(fptr, 256 * i, SEEK_SET);
            fwrite(&byteState, 1, 1, fptr);

            unsigned char byteId = (unsigned char)0;
            fseek(fptr, 256 * i + 1, SEEK_SET);
            fwrite(&byteId, 1, 1, fptr);

            foundProcess ++;
            break;
        }
        
        
    }
    if (foundProcess == 0)
    {
        CR_ERROR = INVALID_PROCESSID_FINISH;
    }
    
}

CrmsFile* cr_open(int process_id, char* file_name, char mode)
{
    int foundProcess = 0;
    int foundFile = 0;
    int foundPCBEntryW = 0;
    for (int i = 0; i < 16; i++)
    {
        if (PCB_TABLE->entriesArray[i]->processId == process_id)
        {
            foundProcess ++;
            if (mode == 'r')
            {
                for (int j = 0; j < 10; j++)
                {
                    // printf("VPN: %i\n", PCB_TABLE->entriesArray[i]->subEntriesArray[j]->VPN);
                    // printf("Offset: %i\n", PCB_TABLE->entriesArray[i]->subEntriesArray[j]->offSet);
                    if (!strcmp((const char *)PCB_TABLE->entriesArray[i]->subEntriesArray[j]->fileName, file_name))
                    {
                        foundFile ++;
                        CrmsFile* crmsFile = CrmsFileInit(PCB_TABLE->entriesArray[i]->processId, 
                                                            PCB_TABLE->entriesArray[i]->subEntriesArray[j]->fileName, 
                                                            PCB_TABLE->entriesArray[i]->subEntriesArray[j]->fileSize);
                        crmsFile->VPN = PCB_TABLE->entriesArray[i]->subEntriesArray[j]->VPN;
                        crmsFile->offSet = PCB_TABLE->entriesArray[i]->subEntriesArray[j]->offSet;
                        crmsFile->lastReadOffset = crmsFile->offSet;
                        crmsFile->lastReadSize = 0;
                        crmsFile->completedPages = 0;
                        // printf("VPN: %i\n", PCB_TABLE->entriesArray[i]->subEntriesArray[j]->VPN);
                        // printf("Offset: %i\n", PCB_TABLE->entriesArray[i]->subEntriesArray[j]->offSet);
                        return crmsFile;
                    }
                    
                }
                
            }
            else if (mode == 'w')
            {
                for (int j = 0; j < 10; j++)
                {
                    if (!strcmp((const char *)PCB_TABLE->entriesArray[i]->subEntriesArray[j]->fileName, file_name))
                    {
                        CrmsFile* crmsFile = CrmsFileInit(PCB_TABLE->entriesArray[i]->processId, 
                                                            PCB_TABLE->entriesArray[i]->subEntriesArray[j]->fileName, 
                                                            PCB_TABLE->entriesArray[i]->subEntriesArray[j]->fileSize);
                        crmsFile->VPN = PCB_TABLE->entriesArray[i]->subEntriesArray[j]->VPN;
                        crmsFile->offSet = PCB_TABLE->entriesArray[i]->subEntriesArray[j]->offSet;
                        crmsFile->lastReadOffset = crmsFile->offSet;
                        crmsFile->lastReadSize = 0;
                        crmsFile->completedPages = 0;
                        return crmsFile;
                    }
                    
                }
                for (int j = 0; j < 10; j++)
                {
                    if (!PCB_TABLE->entriesArray[i]->subEntriesArray[j]->valid)
                    {
                        foundPCBEntryW ++;
                        PCB_TABLE->entriesArray[i]->subEntriesArray[j]->fileSize = 0;
                        PCB_TABLE->entriesArray[i]->subEntriesArray[j]->valid = 1;
                        memcpy(PCB_TABLE->entriesArray[i]->subEntriesArray[j]->fileName, file_name, 12);
                        CrmsFile* crmsFile = CrmsFileInit(PCB_TABLE->entriesArray[i]->processId, 
                                                            PCB_TABLE->entriesArray[i]->subEntriesArray[j]->fileName, 
                                                            PCB_TABLE->entriesArray[i]->subEntriesArray[j]->fileSize);
                        crmsFile->VPN = PCB_TABLE->entriesArray[i]->subEntriesArray[j]->VPN;
                        crmsFile->offSet = PCB_TABLE->entriesArray[i]->subEntriesArray[j]->offSet;
                        crmsFile->lastReadOffset = crmsFile->offSet;
                        crmsFile->lastReadSize = 0;
                        crmsFile->completedPages = 0;

                        // write in memory
                        unsigned char byteValid = (unsigned char)0;
                        fseek(fptr, 256 * i + 21 * j + 14, SEEK_SET);
                        fwrite(&byteValid, 1, 1, fptr);

                        fseek(fptr, 256 * i + 21 * j + 14 + 1, SEEK_SET);
                        fwrite(file_name, 1, 12, fptr);

                        unsigned char fileSize[4] = {0,0,0,0};
                        for (int k = 0; k < 4; k++)
                        {
                            fseek(fptr, 256 * i + 21 * j + 14 + 13 + k, SEEK_SET);
                            fwrite(&fileSize, 1, 4, fptr);
                        }
                        
         

                        return crmsFile;
                    }
                    
                }
                
            }
            else
            {
                CR_ERROR = INVALID_MODE;
            }
            
        }
        
    }
    if (foundProcess == 0 && !CR_ERROR)
    {
        CR_ERROR = INVALID_PROCESSID_OPEN;
    }
    else if (foundFile == 0 && !CR_ERROR)
    {
        CR_ERROR = INVALID_FILE_OPEN;
    }else if (foundPCBEntryW == 0 && !CR_ERROR)
    {
        CR_ERROR = PCB_FULLW_OPEN;
    }
    return NULL;
}

void cr_close(CrmsFile* file_desc)
{
    free(file_desc);
}

void cr_delete_file(CrmsFile* file_desc)
{
    PageTable* pageTable;
    int processIndex;

    for (int i = 0; i < 16; i++)
    {
        if (file_desc->proccessId == PCB_TABLE->entriesArray[i]->processId)
        {
            pageTable = PCB_TABLE->entriesArray[i]->pageTable;
            processIndex = i;
            break;
        }
    }

    for (int j = 0; j < 10; j++)
    {
        if (file_desc->fileName == PCB_TABLE->entriesArray[processIndex]->subEntriesArray[j]->fileName)
        {
            PCB_TABLE->entriesArray[processIndex]->subEntriesArray[j]->valid = 0;
            // Make memory bit invalid
            unsigned char byteValid = (unsigned char)0;
            fseek(fptr, 256 * processIndex + 21 * j + 14, SEEK_SET);
            fwrite(&byteValid, 1, 1, fptr);
            break;
        }
    }
        
    // check which frame each file uses
    unsigned int fileFrames[32];
    int fileCounter = 0;
    unsigned int otherFileFrames[288];
    int otherFileCounter = 0;

    for (int i = 0; i < 10; i++)
    {
        if (!strcmp((const char *)file_desc->fileName, (const char *)PCB_TABLE->entriesArray[processIndex]->subEntriesArray[i]->fileName))
        {
            int currentSize = file_desc->fileSize;
            int currentFreeSpace = 8388608 - file_desc->offSet;
            unsigned int VPN = file_desc->VPN;

            while (currentSize) 
            {
                if (currentSize <= currentFreeSpace) 
                {
                    fileFrames[fileCounter] = pageTable->entriesArray[VPN]->PFN;
                    printf("Ultimo PFN tocado: %i\n", pageTable->entriesArray[VPN]->PFN);
                    currentSize = 0;
                } else 
                {
                    fileFrames[fileCounter] = pageTable->entriesArray[VPN]->PFN;
                    printf("PFN tocado: %i\n", pageTable->entriesArray[VPN]->PFN);
                    VPN++;
                    currentSize = currentSize - currentFreeSpace;
                    currentFreeSpace = 8388608;
                }
                fileCounter++;
            }
        } else {
            int currentSize = PCB_TABLE->entriesArray[processIndex]->subEntriesArray[i]->fileSize;
            int currentFreeSpace = 8388608 - PCB_TABLE->entriesArray[processIndex]->subEntriesArray[i]->offSet;
            unsigned int VPN = PCB_TABLE->entriesArray[processIndex]->subEntriesArray[i]->VPN;

            while (currentSize) 
            {
                if (currentSize <= currentFreeSpace) 
                {
                    otherFileFrames[otherFileCounter] = pageTable->entriesArray[VPN]->PFN;
                    currentSize = 0;
                } else 
                {
                    otherFileFrames[otherFileCounter] = pageTable->entriesArray[VPN]->PFN;
                    VPN++;
                    currentSize = currentSize - currentFreeSpace;
                    currentFreeSpace = 8388608;
                }
                otherFileCounter++;
            }
        }
    }
    if (fileCounter > 2)
    {
        // free all middle frames
        for (int i = 1; i < fileCounter - 1; i++)
        {   
            int frame = fileFrames[i];
            int byteToChange = (int)(frame / 8);
            fseek(fptr, 4096 + byteToChange, SEEK_SET);
            unsigned char byte[1];
            fread(byte, 1, 1, fptr);
            int bitPosition = 7 - (frame - byteToChange*8);
            byte[0] = byte[0] & (~(0x01 << bitPosition));
            fseek(fptr, 4096 + byteToChange, SEEK_SET);
            fwrite(byte, 1, 1, fptr);
            printf("byte: %i, bitPos: %i\n", byteToChange, bitPosition);
            printf("frame liberado: %i\n", frame);
        }

        int freeFirst = 1;
        int freeLast = 1;
        for (int i = 0; i < otherFileCounter; i++)
        {
            if (fileFrames[0] == otherFileFrames[i])
            {
                freeFirst = 0;
            }
            if (fileFrames[fileCounter - 1] == otherFileFrames[i])
            {
                freeLast = 0;
            }
            if (!freeFirst && !freeLast) break;
        }

        if (freeFirst) {
            int frame = fileFrames[0];
            int byteToChange = frame / 8;
            fseek(fptr, 4096 + byteToChange, SEEK_SET);
            unsigned char byte[1];
            fread(byte, 1, 1, fptr);
            int bitPosition = 7 - (frame - byteToChange*3);
            byte[0] = byte[0] & (~(0x01 << bitPosition));
            fwrite(byte, 1, 1, fptr);
        }
        if (freeLast)
        {
            int frame = fileFrames[fileCounter - 1];
            int byteToChange = frame / 8;
            fseek(fptr, 4096 + byteToChange, SEEK_SET);
            unsigned char byte[1];
            fread(byte, 1, 1, fptr);
            int bitPosition = 7 - (frame - byteToChange*3);
            byte[0] = byte[0] & (~(0x01 << bitPosition));
            fwrite(byte, 1, 1, fptr);
        }
    }
    else if (fileCounter == 2) 
    {
        int freeFirst = 1;
        int freeLast = 1;
        for (int i = 0; i < otherFileCounter; i++)
        {
            if (fileFrames[0] == otherFileFrames[i])
            {
                freeFirst = 0;
            }
            if (fileFrames[1] == otherFileFrames[i])
            {
                freeLast = 0;
            }
            if (!freeFirst && !freeLast) break;
        }

        if (freeFirst) {
            int frame = fileFrames[0];
            int byteToChange = frame / 8;
            fseek(fptr, 4096 + byteToChange, SEEK_SET);
            unsigned char byte[1];
            fread(byte, 1, 1, fptr);
            int bitPosition = 7 - (frame - byteToChange*3);
            byte[0] = byte[0] & (~(0x01 << bitPosition));
            fwrite(byte, 1, 1, fptr);
        }
        if (freeLast)
        {
            int frame = fileFrames[fileCounter - 1];
            int byteToChange = frame / 8;
            fseek(fptr, 4096 + byteToChange, SEEK_SET);
            unsigned char byte[1];
            fread(byte, 1, 1, fptr);
            int bitPosition = 7 - (frame - byteToChange*3);
            byte[0] = byte[0] & (~(0x01 << bitPosition));
            fwrite(byte, 1, 1, fptr);
        }
    } else if (fileCounter == 1)
    {
        int freeFirst = 1;
        for (int i = 0; i < otherFileCounter; i++)
        {
            if (fileFrames[0] == otherFileFrames[i])
            {
                freeFirst = 0;
                printf("frame no vacio\n");
                break;
            }
        }
        if (freeFirst) {
            int frame = fileFrames[0];
            int byteToChange = frame / 8;
            fseek(fptr, 4096 + byteToChange, SEEK_SET);
            unsigned char byte[1];
            fread(byte, 1, 1, fptr);
            int bitPosition = 7 - (frame - byteToChange*3);
            byte[0] = byte[0] & (~(0x01 << bitPosition));
            fwrite(byte, 1, 1, fptr);
        }
    }
}


int cr_read(CrmsFile* file_desc, void* buffer, int n_bytes)
{
    PageTable* pageTable;
    unsigned int PFN; 
    int remaining = n_bytes;
    int read = 0;
    unsigned char content[n_bytes];

    if (file_desc->fileSize < n_bytes)
    {
        CR_ERROR = INVALID_READ;
    }
    

    for (int i = 0; i < 16; i++)
    {
        if (file_desc->proccessId == PCB_TABLE->entriesArray[i]->processId && PCB_TABLE->entriesArray[i]->state == 1)
        {
            pageTable = PCB_TABLE->entriesArray[i]->pageTable;
            break;
        }
        
    }
    PFN = pageTable->entriesArray[file_desc->VPN + file_desc->completedPages]->PFN;
    // printf("PFN INICIAL: %i", PFN);
    // printf("PFN: %i\n", PFN);
    // printf("Valid: %i\n", pageTable->entriesArray[file_desc->VPN + file_desc->completedPages]->valid);

    while (file_desc->lastReadSize < file_desc->fileSize)
    {
        if (file_desc->lastReadOffset == 8388608)
        {
            file_desc->completedPages ++;
            file_desc->lastReadOffset = 0;
            PFN = pageTable->entriesArray[file_desc->VPN + file_desc->completedPages]->PFN;
            // printf("PFN CAMBIO: %i", PFN);

        }

        
        fseek(fptr, 4112 + 8388608 * PFN + file_desc->lastReadOffset, SEEK_SET);
        unsigned char bytes[1];
        fread(bytes, 1, 1, fptr);
        content[read] = bytes[0];
        read ++;
        file_desc->lastReadOffset ++;
        file_desc->lastReadSize ++;
        remaining --;
        if (remaining == 0)
        {
            break;
        }
                
    }
    if (file_desc->lastReadSize == file_desc->fileSize)
    {
        file_desc->lastReadOffset = file_desc->offSet;
        file_desc->lastReadSize = 0;
        file_desc->completedPages = 0;
    }
    memcpy(buffer, content, read);
    return read;
}



void cr_strerror(int code)
{
    switch (code) {

        case INVALID_MEM_PATH:
            printf("ERROR: The path of the memory is invalid.\n");
        case INVALID_PROCESSID_EXIST:
            printf("ERROR: The provided process id to search the file doesnt exist.\n");
        case INVALID_PROCESSID_LS:
            printf("ERROR: The provided process id to show its files doesnt exist.\n");
        case PROCESSID_ALREADY_EXIST:
            printf("ERROR: There is already a process with the provided process id in execution.\n");
        case PCB_FULL_START:
            printf("ERROR: There is no space available in the PCB to start your process.\n");
        case INVALID_PROCESSID_FINISH:
            printf("ERROR: There is no process running with the provided id.\n");        
        case INVALID_PROCESSID_OPEN:
            printf("ERROR: The process selected to open the file was not found.\n");
        case INVALID_MODE:
            printf("ERROR: The mode selected for opening the file is invalid.\n");
        case INVALID_FILE_OPEN:
            printf("ERROR: The file selected to open was not found.\n");
        case PCB_FULLW_OPEN:
            printf("ERROR: There is no space available in the PCB to create a new file.\n");
        case INVALID_READ:
            printf("ERROR: The file you are trying to read is smaller than your buffer.\n");


    }
}