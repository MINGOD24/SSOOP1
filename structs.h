#pragma once

typedef struct pcbsubentry
{
    unsigned int valid;
    unsigned char* fileName; // length 12 
    unsigned int fileSize;
    unsigned int virtualAddress;

} PCBSubEntry;


typedef struct pcbentry
{
    unsigned int state;
    unsigned int processId;
    unsigned char* processName; // length 12 
    PCBSubEntry** subEntriesArray; // length 10
    PageTable* pageTable;

} PCBEntry;

typedef struct pcbtable
{
    PCBEntry** entriesArray;

} PCBTable;


typedef struct pagetableentry
{
    unsigned int valid;
    unsigned int PFN;  


} PageTableEntry;

typedef struct pagetable
{
    PageTableEntry** entriesArray;

} PageTable;


PCBSubEntry* PCBSubEntryInit(unsigned char* fileName, unsigned int fileSize, unsigned int virtualAddress);
PCBEntry* PCBEntryInit(unsigned int processId, unsigned char* processName, PCBSubEntry** subEntriesArray, PageTable* pageTable);
PCBTable* PCBTableinit(PCBEntry** entriesArray);
PageTableEntry* PageTableEntryInit(unsigned int PFN);
PageTable* PageTableInit(PageTableEntry** entriesArray);
