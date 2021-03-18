////////////////////////////////////////////
//Main file: 537pfsim-fifo.c, 537pfsim-lru.c, 537pfsim-clock.c
//This file: PageTable.h
//Semester: CS537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
////////////////////////////////////////////

#ifndef PAGETABLE_H_INCLUDED
#define PAGETABLE_H_INCLUDED

typedef struct PageTableEntry{
    unsigned long vpn;
    int ppn;
    struct PageTableEntry* nextEntry;//pointer to the next entry in the bucket
} PageTableEntry;

typedef struct{
    int tableSize;
    unsigned long pid;
    PageTableEntry** pageTableEntry;//hash table
} HashPageTable;

HashPageTable* initPageTable(int pageFrameNum, TraceFileInfo* traceFileInfo);

int hash(unsigned long vpn, int tableSize);

int tableNumOf(HashPageTable* hashPageTables, unsigned long pid);

int getPpn(HashPageTable* hashPageTables, unsigned long pid, unsigned long vpn);

void insertPTEntry(HashPageTable* hashPageTables, unsigned long pid, unsigned long vpn, int pageFrameNum);

void deletePTEntry(HashPageTable* hashPageTables, unsigned long pid, unsigned long vpn);

void cleanProcPT(HashPageTable* hashPageTables, unsigned long pid);

void recyclePageTable(HashPageTable* hashPageTables);

#endif
