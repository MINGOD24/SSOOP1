#pragma once


typedef struct pcbsubentry
{
    unsigned int valid;
    unsigned char fileName[12];
    unsigned int fileSize;
    unsigned int virtualAddress;

} PCBSubEntry;


typedef struct pcbentry
{
    unsigned int state;
    unsigned int processId;
    unsigned char processName[12];
    PCBSubEntry* subEntriesArray[10];
    PageTable* pageTable;

} PCBEntry;

typedef struct pcbtable
{
    PCBEntry* entriesArray;

} PCBTable;


typedef struct pagetableentry
{
    unsigned int valid;
    unsigned int PFN;  


} PageTableEntry;

typedef struct pagetable
{
    PageTableEntry* entriesArray;

} PageTable;


typedef struct crms
{
    pcbtable* pcbTable;

} CRMS;

PCBSubEntry* pcbsubentry_init(unsigned char fileName[], unsigned int fileSize, unsigned int virtualAddress);