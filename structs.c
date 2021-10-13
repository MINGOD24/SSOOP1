#include "structs.h"

PCBSubEntry* PCBSubEntryInit(unsigned char* fileName, unsigned int fileSize, unsigned int virtualAddress) {
    PCBSubEntry* pcbSubEntry = malloc(sizeof(PCBSubEntry));
    pcbSubEntry->valid = 0;
    pcbSubEntry->fileName = fileName;
    pcbSubEntry->fileSize = fileSize;
    pcbSubEntry->virtualAddress = virtualAddress;

    return pcbSubEntry;
}

PCBEntry* PCBEntryInit(unsigned int processId, unsigned char* processName, PCBSubEntry** subEntriesArray, PageTable* pageTable) {
    PCBEntry* pcbEntryInit = malloc(sizeof(PCBEntryInit));
    pcbEntryInit->state = 0;
    pcbEntryInit->processId = processId;
    pcbEntryInit->processName = processName;
    pcbEntryInit->subEntriesArray = subEntriesArray;
    pcbEntryInit->pageTable = pageTable;

    return pcbEntryInit;
}

PCBTable* PCBTableInit(PCBEntry** entriesArray) {
    PCBTable* pcbTable = malloc(sizeof(PCBTable));
    pcbTable->entriesArray = entriesArray;

    return pcbTable;
}

PageTableEntry* PageTableEntryInit(unsigned int PFN) {
    PageTableEntry* pageTableEntry = malloc(sizeof(PageTableEntry));
    pageTableEntry->valid = 0;
    pageTableEntry->PFN = PFN;

    return pageTableEntry;
}

PageTable* PageTableInit(PageTableEntry** entriesArray) {
    PageTable* pageTable = malloc(sizeof(PageTable));
    pageTable->entriesArray = entriesArray;

    return pageTable;
}
