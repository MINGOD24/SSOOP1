#pragma once

typedef struct pcbsubentry
{
    unsigned int valid;
    unsigned char fileName[12]; // length 12 
    unsigned int fileSize;
    unsigned int VPN;
    unsigned int offSet;

} PCBSubEntry;


typedef struct pagetableentry
{
    unsigned int valid;
    unsigned int PFN;  


} PageTableEntry;

typedef struct pagetable
{
    PageTableEntry* entriesArray[32];

} PageTable;


typedef struct pcbentry
{
    unsigned int state;
    unsigned int processId;
    unsigned char processName[12]; // length 12 
    PCBSubEntry* subEntriesArray[10]; // length 10
    PageTable* pageTable;

} PCBEntry;

typedef struct pcbtable
{
    PCBEntry* entriesArray[16];

} PCBTable;


typedef struct crmsfile
{
    unsigned int proccessId;
    unsigned char fileName[12];
    unsigned int fileSize;
    unsigned int VPN;
    unsigned int offSet;
    unsigned int lastReadOffset;
    unsigned int completedPages;
    unsigned int lastReadSize;
    

} CrmsFile;


typedef enum errorcode
{
    //Write all the errors below
    INVALID_MEM_PATH,
    INVALID_MODE,
    PCB_FULL,
    INVALID_PROCESS_OPENING_FILE,
    INVALID_FILE_OPENING_FILE,
    INVALID_FINISH_PROCESS,

} ErrorCode;




PCBSubEntry* PCBSubEntryInit(unsigned char discPCBTable[], int i, int j);
PCBEntry* PCBEntryInit(unsigned char discPCBTable[], int i);
PCBTable* PCBTableInit();
PageTableEntry* PageTableEntryInit(unsigned char discPCBTable[], int i, int j);
PageTable* PageTableInit(unsigned char discPCBTable[], int i);
CrmsFile* CrmsFileInit(unsigned int proccessId, unsigned char fileName[], unsigned int fileSize);

ErrorCode CR_ERROR;