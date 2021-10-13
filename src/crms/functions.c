#include <unistd.h> // Symbolic Constants - access
#include "functions.h"
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <stdlib.h>
#include "structs.h"
#include<string.h>  


char* VIRTUAL_MEMORY_PATH;
FILE *fptr;
PCBTable* PCB_TABLE;


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
    for (int i = 0; i < 16; i++)
    {
        if (!strcmp((const char *)PCB_TABLE->entriesArray[i]->processName, "") && !PCB_TABLE->entriesArray[i]->processId)
        {
            memcpy(PCB_TABLE->entriesArray[i]->processName, process_name, 12);
            PCB_TABLE->entriesArray[i]->processId = process_id;
            PCB_TABLE->entriesArray[i]->state = 1;
            break;
        }
        
    }
    
}

//TODO: Falta liberar frames
void cr_finish_process(int process_id)
{
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
            break;
        }
        
        
    }
}