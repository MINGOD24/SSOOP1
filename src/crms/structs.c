#include "structs.h"
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <stdlib.h>
#include <string.h> // strtok, strcpy, etc.

extern FILE *fptr;

PCBSubEntry* PCBSubEntryInit(unsigned char discPCBTable[], int i, int j) {
    PCBSubEntry* pcbSubEntry = malloc(sizeof(PCBSubEntry));
    
    unsigned char firstByte = discPCBTable[256 * i + 21 * j + 14];

    pcbSubEntry->valid = firstByte;

    for (int k = 0; k < 12; k++)
    {
        pcbSubEntry->fileName[k] = discPCBTable[(256 * i + 21 * j + 14) + k + 1];
    }
    
    unsigned char fileSize[4];
    for (int k = 0; k < 4; k++)
    {
        fileSize[k] = discPCBTable[(256 * i + 21 * j + 14) + k + 12] << (3 - k)*8;
    }
    
    unsigned int resultUnitedBytes  = fileSize[11] | fileSize[10] | fileSize[9]
                            | fileSize[8] | fileSize[7] | fileSize[6]
                            | fileSize[5] | fileSize[4] | fileSize[3]
                            | fileSize[2] | fileSize[1] | fileSize[0];
    pcbSubEntry->fileSize = resultUnitedBytes;

    unsigned char eighteenByte = discPCBTable[(256 * i + 21 * j + 14 + 17)];
    unsigned char nineteenByte = discPCBTable[(256 * i + 21 * j + 14 + 17 + 1)];
    unsigned char twentyByte = discPCBTable[(256 * i + 21 * j + 14 + 17 + 2)];
    unsigned char twentyOneByte = discPCBTable[(256 * i + 21 * j + 14 + 17 + 3)];


    unsigned int VPN = (eighteenByte << 1) | (nineteenByte >> 7);
    unsigned int offSet = ((nineteenByte && 127) << 16) | (twentyByte << 8) | (twentyOneByte);

    pcbSubEntry->VPN = VPN;
    pcbSubEntry->offSet = offSet;

    return pcbSubEntry;
}

PCBEntry* PCBEntryInit(unsigned char discPCBTable[], int i) {
    PCBEntry* pcbEntryInit = malloc(sizeof(PCBEntry));

    unsigned char firstByte = discPCBTable[256 * i];
    unsigned char secondByte = discPCBTable[1 + 256 * i];

    pcbEntryInit->state = (unsigned int) firstByte;
    pcbEntryInit->processId = (unsigned int) secondByte; 

    for (int j = 0; j < 12; j++)
    {
        pcbEntryInit->processName[j] = discPCBTable[(i * 256) + j + 2];
    }
    

    for (int j = 0; j < 10; j++)
    {
        pcbEntryInit->subEntriesArray[j] = PCBSubEntryInit(discPCBTable, i, j);
        
    }

    PageTable* pageTable = PageTableInit(discPCBTable, i);
    pcbEntryInit->pageTable = pageTable;
    
    
    return pcbEntryInit;
}

PCBTable* PCBTableInit() {
    PCBTable* pcbTable = malloc(sizeof(PCBTable));
    
    unsigned char discPCBTable[4096];
    
    fseek(fptr, 0, SEEK_SET);
    fread(discPCBTable, 1, 4096, fptr);



    for (int i = 0; i < 16; i++)
    {
        pcbTable->entriesArray[i] = PCBEntryInit(discPCBTable, i);
    }
    return pcbTable;
}

PageTableEntry* PageTableEntryInit(unsigned char discPCBTable[], int i, int j) {
    PageTableEntry* pageTableEntry = malloc(sizeof(PageTableEntry));
    
    unsigned char firstByte = discPCBTable[256 * i + 210 + 14 + j];

    unsigned int valid = (firstByte & (1 << 7)) >> 7 ;
    unsigned int PFN = (firstByte & 127);

    pageTableEntry->valid = valid;
    pageTableEntry->PFN = PFN;

    return pageTableEntry;
}

PageTable* PageTableInit(unsigned char discPCBTable[], int i) {
    PageTable* pageTable = malloc(sizeof(PageTable));
    
    for (int j = 0; j < 32; j++)
    {
        pageTable->entriesArray[j] = PageTableEntryInit(discPCBTable, i, j);
    }

    return pageTable;
}

CrmsFile* CrmsFileInit(unsigned int proccessId, unsigned char fileName[], unsigned int fileSize) {
    CrmsFile* crmsFile = malloc(sizeof(CrmsFile));
    
    crmsFile->proccessId = proccessId;
    memcpy(crmsFile->fileName, fileName, 12);
    crmsFile->fileSize = fileSize;

    return crmsFile;
}
