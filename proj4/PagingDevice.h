////////////////////////////////////////////
//Main file: 537pfsim-fifo.c, 537pfsim-lru.c, 537pfsim-clock.c
//This file: PagingDevice.h
//Semester: CS537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
////////////////////////////////////////////

#ifndef PAGINGDEVICE_H_INCLUDED
#define PAGINGDEVICE_H_INCLUDED

enum PROCSTATE{
    RUNNABLE, BLOCKED, END
};

enum REPPOL{
    FIFO, LRU, CLOCK
};

typedef struct ReadReq{
    int procNum;
    unsigned long pid;
    unsigned long vpn;
    int time;// the time left for the I/O process
    int procState;
    long nextLine;// the next line to run in the trace file
    int curBlock;// current trace file block of the process
    struct ReadReq* nextReq;
} ReadReq;

typedef struct{
    int pidNum;
    int reqQueueNum; 
    ReadReq** readReqs;
    ReadReq* reqQueue;
    ReadReq* reqQueueTail;
    int curProc;
} PagingDevice;

PagingDevice* initPagingDevice(TraceFileInfo* metadata);

void readReq(PagingDevice* pagingDevice, unsigned long vpn, unsigned long ppn);

void timeFlow(PagingDevice* pagingDevice, RAM* mem, HashPageTable* hashPageTables, FILE* tracefile, int repPol);

void contextSwitch(PagingDevice* pagingDevice, FILE* tracefile);

long skipReadTime(PagingDevice* pagingDevice, long clock);

void recyclePagingDevice(PagingDevice* pagingDevice);

void recycleAll(TraceFileInfo* metadata, HashPageTable* hashPageTables, RAM* mem, PagingDevice* pagingDevice);

#endif
