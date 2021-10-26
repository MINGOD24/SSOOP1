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
    NO_ERROR,
    INVALID_MEM_PATH,
    INVALID_PROCESSID_EXIST,
    INVALID_PROCESSID_LS,
    PROCESSID_ALREADY_EXIST,
    PCB_FULL_START,
    INVALID_PROCESSID_FINISH,
    INVALID_PROCESSID_OPEN,
    INVALID_MODE,
    INVALID_FILE_OPEN,
    PCB_FULLW_OPEN,
    INVALID_READ


} ErrorCode;




PCBSubEntry* PCBSubEntryInit(unsigned char discPCBTable[], int i, int j);
PCBEntry* PCBEntryInit(unsigned char discPCBTable[], int i);
PCBTable* PCBTableInit();
PageTableEntry* PageTableEntryInit(unsigned char discPCBTable[], int i, int j);
PageTable* PageTableInit(unsigned char discPCBTable[], int i);
CrmsFile* CrmsFileInit(unsigned int proccessId, unsigned char fileName[], unsigned int fileSize);
void destroy_all();

ErrorCode CR_ERROR;