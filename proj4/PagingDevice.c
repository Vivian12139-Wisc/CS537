////////////////////////////////////////////
//Main file: 537pfsim-fifo.c, 537pfsim-lru.c, 537pfsim-clock.c
//This file: PagingDevice.c
//Semester: CS537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
////////////////////////////////////////////

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "CollectInfo.h"
#include "PageTable.h"
#include "RAM.h"
#include "PagingDevice.h"
const int READTIME = 2000000;

/*
 * This function initializes the paging device
 * @arg the metadata of trace file information
 * @return the paging device that has been initialized
 */
PagingDevice* initPagingDevice(TraceFileInfo* metadata){
    PagingDevice* pagingDevice;
    int pidNum = metadata->pidNum;
    if ((pagingDevice = malloc(sizeof(PagingDevice))) == NULL ||
    (pagingDevice->readReqs = malloc(sizeof(ReadReq*) * pidNum)) == NULL){
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    pagingDevice->pidNum = pidNum;
    pagingDevice->reqQueue = NULL;
    pagingDevice->reqQueueTail = NULL;
    pagingDevice->curProc = -1;
    // initialize reqQueueNum
    pagingDevice->reqQueueNum = 0; 
    for (int i = 0; i < pidNum; i++){
        if ((pagingDevice->readReqs[i] = malloc(sizeof(ReadReq))) == NULL){
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        pagingDevice->readReqs[i]->procNum = i;
        pagingDevice->readReqs[i]->pid = metadata->pids[i];
        pagingDevice->readReqs[i]->vpn = (unsigned long) 0;
        pagingDevice->readReqs[i]->time = -1;
        pagingDevice->readReqs[i]->procState = RUNNABLE;
        pagingDevice->readReqs[i]->nextLine = metadata->blocks[i][0].start;
        pagingDevice->readReqs[i]->curBlock = 0;
        pagingDevice->readReqs[i]->nextReq = NULL;
    }
    return pagingDevice;
}

/*
 * This function sends an I/O request to the end of I/O queue
 * It blocks the process
 * @arg pagingDevice 
 * @arg pid the pid of I/O request
 * @arg vpn the virtual page number 
 */
void readReq(PagingDevice* pagingDevice, unsigned long pid, unsigned long vpn){
    int procNum = -1;
    for (int i = 0; i < pagingDevice->pidNum; i++){
        if (pagingDevice->readReqs[i]->pid == pid){
            procNum = i;
            break;
        }
    }
    if (procNum == -1){
        printf("Error: process %lu not found in the paging device\n", pid);
        exit(EXIT_FAILURE);
    }
    ReadReq* targetReq = pagingDevice->readReqs[procNum];
    if (targetReq->procState != RUNNABLE ||
    targetReq->time > 0){
        printf("Error: running unrunnable process\n");
        exit(EXIT_FAILURE);
    }
    targetReq->vpn = vpn;
    targetReq->time = READTIME;
    targetReq->procState = BLOCKED;
    targetReq->nextReq = NULL;
    if (pagingDevice->reqQueue == NULL){
        pagingDevice->reqQueue = targetReq;
        pagingDevice->reqQueueTail = targetReq;
        targetReq->time++;
    } else {
        pagingDevice->reqQueueTail->nextReq = targetReq;
        pagingDevice->reqQueueTail = targetReq;
    }
    pagingDevice->reqQueueNum ++;
}

/*
 * This function informs the disk that 1ns has past
 * If an I/O completed, unblock the process, start the next I/O and make a context switch
 * @arg pagingDevice the paging device
 * @arg mem RAM
 * @arg hashPageTables the array of all process' hash page tables
 * @arg tracefile the trace file 
 * @arg repPol
 */
void timeFlow(PagingDevice* pagingDevice, RAM* mem, HashPageTable* hashPageTables, FILE* tracefile, int repPol){
    if (pagingDevice->reqQueue == NULL)
        return;
    ReadReq* targetReq = pagingDevice->reqQueue;
    targetReq->time--;
    if (targetReq->time == 0){
        targetReq->procState = RUNNABLE;
        pagingDevice->reqQueue = targetReq->nextReq;
        targetReq->nextReq = NULL;
        if (pagingDevice->reqQueue == NULL)
            pagingDevice->reqQueueTail = NULL;
        PageInfo pageInfo;
        switch (repPol){
            case FIFO:
                pageInfo = FIFO_LRURead(mem, targetReq->pid, targetReq->vpn);
                break;
            case LRU:
                pageInfo = FIFO_LRURead(mem, targetReq->pid, targetReq->vpn);
                break;
            case CLOCK:
                pageInfo = clockRead(mem, targetReq->pid, targetReq->vpn);
                break;
            default:
                printf("Error: unrecognized replacement policy\n");
                exit(EXIT_FAILURE);
                break;
        }
        if (pageInfo.replacedpid != 0 || pageInfo.replacedvpn != 0)
            deletePTEntry(hashPageTables, pageInfo.replacedpid, pageInfo.replacedvpn);
        insertPTEntry(hashPageTables, targetReq->pid, targetReq->vpn, pageInfo.ppn);
        contextSwitch(pagingDevice, tracefile);
    pagingDevice->reqQueueNum --;
    }
}

/*
 * This function makes a context switch to the runnable process with earliest unprocessed trace
 * @arg pagingDevice the paging device 
 * @arg tracefile the tracefile 
 */
void contextSwitch(PagingDevice* pagingDevice, FILE* tracefile){
    int procNum = -1;
    long earliest = -1;
    for (int i = 0; i < pagingDevice->pidNum; i++){
        if (pagingDevice->readReqs[i]->procState == RUNNABLE &&
        (earliest == (long) -1 || pagingDevice->readReqs[i]->nextLine < earliest)){
            procNum = i;
            earliest = pagingDevice->readReqs[i]->nextLine;
        }
    }
    pagingDevice->curProc = procNum;
    if (procNum != -1)
        fseek(tracefile, earliest, SEEK_SET);
}

/*
 * This function skips all I/O time of current I/O
 * @arg pagingDevice the pagingDevice to be skipped time
 * @arg clock the current clock time
 * @return the new clock time 
 */
long skipReadTime(PagingDevice* pagingDevice, long clock){
    int time = pagingDevice->reqQueue->time;
    pagingDevice->reqQueue->time = 1;
    return clock + time - 1;
}

/*
 * This function recycles the whole paging device
 * @arg the paging device to be recycled
 */
void recyclePagingDevice(PagingDevice* pagingDevice){
    for (int i = 0 ; i < pagingDevice->pidNum ; i++)
        free(pagingDevice->readReqs[i]);
    free(pagingDevice->readReqs);
    free(pagingDevice);
}

/*
 * This function recycles all components of the program
 * @arg metadata the metadat of TraceFileInfomation to be recycled
 * @arg hashPageTables the array of hash tables to be recycled
 * @arg mem the RAM to be recycled 
 * @arg the paging device to be recycled
 */
void recycleAll(TraceFileInfo* metadata, HashPageTable* hashPageTables, RAM* mem, PagingDevice* pagingDevice){
    recycleInfo(metadata);
    recyclePageTable(hashPageTables);
    recycleRAM(mem);
    recyclePagingDevice(pagingDevice);
}
