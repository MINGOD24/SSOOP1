#include <unistd.h> // Symbolic Constants - access
#include "functions.h"
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <stdlib.h>
#include "structs.h"
#include<string.h>  


char* VIRTUAL_MEMORY_PATH;
FILE *fptr;
PCBTable* PCB_TABLE;

extern ErrorCode CR_ERROR;

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
    return 0;
}


void cr_ls_files(int process_id)
{
    for (int i = 0; i < 16; i++)
    {
        
        if (PCB_TABLE->entriesArray[i]->processId == process_id)
        {
            for (int j = 0; j < 10; j++)
            {
                if (PCB_TABLE->entriesArray[i]->subEntriesArray[j]->valid)
                {
                    printf("%s\n", PCB_TABLE->entriesArray[i]->subEntriesArray[j]->fileName);
                }
            }
            
        }
        
        
    }
}

void cr_start_process(int process_id, char* process_name)
{
    int found = 0;
    for (int i = 0; i < 16; i++)
    {
        if (!strcmp((const char *)PCB_TABLE->entriesArray[i]->processName, "") && !PCB_TABLE->entriesArray[i]->processId)
        {
            found ++;
            memcpy(PCB_TABLE->entriesArray[i]->processName, process_name, 12);
            PCB_TABLE->entriesArray[i]->processId = process_id;
            PCB_TABLE->entriesArray[i]->state = 1;
            break;
        }
        
    }
    if (!found)
    {
        CR_ERROR = PROCCESS_NOT_FOUND_STARTING;
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
            }
            
            PCB_TABLE->entriesArray[i]->state = 0;
            PCB_TABLE->entriesArray[i]->processId = 0;
            foundProcess ++;
            break;
        }
        
        
    }
    if (foundProcess == 0)
    {
        CR_ERROR = INVALID_FINISH_PROCESS;
    }
    
}

CrmsFile* cr_open(int process_id, char* file_name, char mode)
{
    int foundProcess = 0;
    int foundFile = 0;
    for (int i = 0; i < 16; i++)
    {
        if (PCB_TABLE->entriesArray[i]->processId == process_id)
        {
            foundProcess ++;
            if (mode == 'r')
            {
                for (int j = 0; j < 10; j++)
                {
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
                        foundFile ++;
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
                    if (PCB_TABLE->entriesArray[i]->subEntriesArray[j]->valid == 0)
                    {
                        foundFile ++;
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
    if (foundProcess == 0)
    {
        CR_ERROR = INVALID_PROCESS_OPENING_FILE;
    }
    else if (foundFile == 0)
    {
        CR_ERROR = INVALID_FILE_OPENING_FILE;
    }
    
    
}

void cr_close(CrmsFile* file_desc)
{
    free(file_desc);
}

void cr_delete_file(CrmsFile* file_desc)
{
    int* fileFrames[16];
    int filesCounter = 0;
    int** otherFilesFrames[144];
    int otherFilesCounter = 0;
    PageTable* pageTable;

    for (int i = 0; i < 16; i++)
    {
        if (file_desc->proccessId == PCB_TABLE->entriesArray[i]->processId)
        {
            pageTable = PCB_TABLE->entriesArray[i]->pageTable;

            for (int j = 0; j < 10; j++)
            {   
                PCBSubEntry* subEntry = PCB_TABLE->entriesArray[i]->subEntriesArray[j];
                unsigned int VFN = pageTable->entriesArray[subEntry->VPN];

                if (file_desc->fileName == PCB_TABLE->entriesArray[i]->subEntriesArray[j]->fileName)
                {
                    PCB_TABLE->entriesArray[i]->subEntriesArray[j]->valid = 0;

                    int currentOffset = subEntry->offSet;
                    int frameFreeSpace = 8388608 - currentOffset;
                    int currentSize = subEntry->fileSize;

                    while (currentSize)
                    {
                        if (currentSize <= frameFreeSpace) {
                            fileFrames[filesCounter] = VFN;  
                            currentSize = 0;  
                        } else {
                            currentSize = currentSize - frameFreeSpace;
                            frameFreeSpace = 8388608;
                            fileFrames[filesCounter] = VFN;  
                        }
                        filesCounter++;
                    }

                } else {

                    int currentOffset = subEntry->offSet;
                    int frameFreeSpace = 8388608 - currentOffset;
                    int currentSize = subEntry->fileSize;

                    while (currentSize)
                    {
                        if (currentSize <= frameFreeSpace) {
                            otherFilesFrames[otherFilesCounter] = VFN;  
                            currentSize = 0;  
                        } else {
                            currentSize = currentSize - frameFreeSpace;
                            frameFreeSpace = 8388608;
                            otherFilesFrames[otherFilesCounter] = VFN;  
                        }
                        otherFilesCounter++;
                    }
                }
            }
            if (filesCounter < 16) {
                fileFrames[filesCounter + 1] = NULL;
            }
            if (otherFilesCounter < 144) {
                otherFilesFrames[otherFilesCounter + 1] = NULL;
            }
            break;
        }
        
    }
    // HACER COMPARACION 
    


}

int cr_read(CrmsFile* file_desc, void* buffer, int n_bytes)
{
    PageTable* pageTable;
    unsigned int PFN; 
    int remaining = n_bytes;
    int read = 0;
    unsigned char content[n_bytes];

    for (int i = 0; i < 16; i++)
    {
        if (file_desc->proccessId == PCB_TABLE->entriesArray[i]->processId)
        {
            pageTable = PCB_TABLE->entriesArray[i]->pageTable;
            break;
        }
        
    }

    PFN = pageTable->entriesArray[file_desc->VPN + file_desc->completedPages]->PFN;

    printf("VPN: %i", file_desc->VPN);

    while (file_desc->lastReadSize < file_desc->fileSize)
    {
        if (file_desc->lastReadOffset == 8388608)
        {
            file_desc->completedPages ++;
            file_desc->lastReadOffset = 0;
            PFN = pageTable->entriesArray[file_desc->VPN + file_desc->completedPages]->PFN;

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
    
    memcpy(buffer, content, read);
    return read;
}



void cr_strerror(int code)
{
    switch (code) {

        case INVALID_MEM_PATH:
            printf("ERROR: The path of the virtual memory is invalid.\n");
        case INVALID_MODE:
            printf("EEROR: The mode selected for opening the file is invalid.\n");
        case PROCCESS_NOT_FOUND_STARTING:
            printf("ERROR: The procces selected to start was not found.\n");
        case INVALID_PROCESS_OPENING_FILE:
            printf("ERROR: The process selected to open the file was not found.\n");
        case INVALID_FILE_OPENING_FILE:
            printf("ERROR: The file selected to open was not found.\n");
        case INVALID_FINISH_PROCESS:
            printf("ERROR: The process selected to finis was not found.\n");

    }
}