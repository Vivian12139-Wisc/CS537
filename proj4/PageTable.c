////////////////////////////////////////////
//Main file: 537pfsim-fifo.c, 537pfsim-lru.c, 537pfsim-clock.c
//This file: PageTable.c
//Semester: CS537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
////////////////////////////////////////////

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "CollectInfo.h"
#include "PageTable.h"

/*
 * This function initializes the page tables
 * @arg pageFrameNum the number of page frams in RAM
 * @arg traceFileInfo the information of tracefile
 * @return the array of hashtable that have been initialized
 */
HashPageTable* initPageTable(int pageFrameNum, TraceFileInfo* traceFileInfo){
    HashPageTable* hashPageTables;
    int tableSize = (int)(pageFrameNum * 1.2) + 1;
    int procNum = traceFileInfo->pidNum;
    if ((hashPageTables = malloc(sizeof(HashPageTable) * (procNum + 1))) == NULL){
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for(int proc = 0; proc < procNum; proc++){
        hashPageTables[proc].tableSize = tableSize;
        hashPageTables[proc].pid = traceFileInfo->pids[proc];
        if ((hashPageTables[proc].pageTableEntry = malloc(sizeof(PageTableEntry*) * tableSize)) == NULL){
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < tableSize; i++)
            hashPageTables[proc].pageTableEntry[i] = NULL;
    }
    // the last page table is an empty mark
    hashPageTables[procNum].tableSize = 0;
    hashPageTables[procNum].pid = (unsigned long) 0;
    hashPageTables[procNum].pageTableEntry = NULL;
    return hashPageTables;
}

/*
 * This function returns the has index of page tables
 * @arg vpn the vip field of page table
 * @arg tableSize the size of hashtable
 * @return the hash index of page tables
 */
int hash(unsigned long vpn, int tableSize){
    return (int)(vpn % tableSize);
}

/*
 * This function returns the table number of certain process
 * It reports error and exits if no such process's found
 * @arg hashPageTables the array of all processes' hash page tables
 * @arg pid the certain process
 * @return the table number of certain process
 */
int tableNumOf(HashPageTable* hashPageTables, unsigned long pid){
    int search = 0;
    while (hashPageTables[search].tableSize != 0){
        if (hashPageTables[search].pid == pid)
            return search;
        search++;
    }
    printf("Page table for pid %lu not correctly created\n", pid);
    exit(EXIT_FAILURE);
    return -1;
}

/*
 * This function looks up the page tables and return the physical page number
 * It returns -1 if no such (pid, vpn) combination's found
 * @argc hashPageTables the array of all processes' hash tables
 * @argc pid the pid of certain process
 * @argc vpn the virtual page number 
 * @return the physical page number
 */
int getPpn(HashPageTable* hashPageTables, unsigned long pid, unsigned long vpn){
    int tableNum = tableNumOf(hashPageTables, pid);
    int tableSize = hashPageTables[tableNum].tableSize;
    int entryNum = hash(vpn, tableSize);
    PageTableEntry* tableEntry = hashPageTables[tableNum].pageTableEntry[entryNum];
    while (tableEntry != NULL){
        if (tableEntry->vpn == vpn)
            return tableEntry->ppn;
        else
            tableEntry = tableEntry->nextEntry;
    }
    return -1;
}

/*
 * This function inserts a page table entry
 * @arg hashPageTables the array of all process' hash page tables
 * @arg pid the pid of page table entry to be deleted
 * @arg vpn the virtual page number to be deleted
 */
void insertPTEntry(HashPageTable* hashPageTables, unsigned long pid, unsigned long vpn, int pageFrameNum){
    int tableNum = tableNumOf(hashPageTables, pid);
    int tableSize = hashPageTables[tableNum].tableSize;
    int entryNum = hash(vpn, tableSize);
    PageTableEntry** tableEntry = &(hashPageTables[tableNum].pageTableEntry[entryNum]);
    while (*tableEntry != NULL)
        tableEntry = &((*tableEntry)->nextEntry);
    
    if ((*tableEntry = malloc(sizeof(PageTableEntry))) == NULL){
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    (*tableEntry)->vpn = vpn;
    (*tableEntry)->ppn = pageFrameNum;
    (*tableEntry)->nextEntry = NULL;
}

/*
 * This function deletes a page table entry
 * @arg hashPageTables the array of all process' hash page tables
 * @arg pid the pid of page table entry to be deleted
 * @arg vpn the virtual page number to be deleted 
 */
void deletePTEntry(HashPageTable* hashPageTables, unsigned long pid, unsigned long vpn){
    int tableNum = tableNumOf(hashPageTables, pid);
    int tableSize = hashPageTables[tableNum].tableSize;
    int entryNum = hash(vpn, tableSize);
    PageTableEntry** tableEntry = &(hashPageTables[tableNum].pageTableEntry[entryNum]);
    while (*tableEntry != NULL){
        if ((*tableEntry)->vpn == vpn){
            PageTableEntry* nextEntry = (*tableEntry)->nextEntry;
            free(*tableEntry);
            *tableEntry = nextEntry;
            return;
        }
        tableEntry = &((*tableEntry)->nextEntry);
    }
    printf("Error: Page table entry (%lu, %lu) not found\n", pid, vpn);
    exit(EXIT_FAILURE);
}

/*
 * This function cleans all table entries that belong to a certain process
 * @arg hashPageTables the array of all process' hash page tables
 * @arg pid the certain process
 */
void cleanProcPT(HashPageTable* hashPageTables, unsigned long pid){
    int tableNum = tableNumOf(hashPageTables, pid);
    int tableSize = hashPageTables[tableNum].tableSize;
    for (int i = 0; i < tableSize; i++){
        PageTableEntry* tableEntry = hashPageTables[tableNum].pageTableEntry[i];
        PageTableEntry* nextEntry = NULL;
        while (tableEntry != NULL){
            nextEntry = tableEntry->nextEntry;
            free(tableEntry);
            tableEntry = nextEntry;
        }
        hashPageTables[tableNum].pageTableEntry[i] = NULL;
    }
}

/*
 * This function recycles the page tables
 * @arg hashPageTables the array of all process's hash page tables
 */
void recyclePageTable(HashPageTable* hashPageTables){
    int tableNum = 0;
    while (hashPageTables[tableNum].tableSize != 0){
        for (int i = 0; i < hashPageTables[tableNum].tableSize; i++){
            PageTableEntry* tableEntry = hashPageTables[tableNum].pageTableEntry[i];
            PageTableEntry* nextEntry = NULL;
            while (tableEntry != NULL){
                nextEntry = tableEntry->nextEntry;
                free(tableEntry);
                tableEntry = nextEntry;
            }
        }
        free(hashPageTables[tableNum].pageTableEntry);
        tableNum++;
    }
    free(hashPageTables);
}
