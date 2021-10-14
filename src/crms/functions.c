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