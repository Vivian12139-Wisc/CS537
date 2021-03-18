////////////////////////////////////////////
//Main file: 537pfsim-fifo.c, 537pfsim-lru.c, 537pfsim-clock.c
//This file: RAM.h
//Semester: CS537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
////////////////////////////////////////////

#ifndef RAM_H_INCLUDED
#define RAM_H_INCLUDED

typedef struct MemFrame{
    int ppn;
    int refbit;
    unsigned long pid;
    unsigned long vpn;
    struct MemFrame* nextFrame;
    struct MemFrame* lastFrame;
} MemFrame;

typedef struct{
    int numOfFrame;
    MemFrame* freeFrames;// the start of all free frames, as doubly circular linked list
    MemFrame* usedFrames;// the start of all used frames, as doubly circular linked list
    int usedFrameNum; // the number of frames that are occupied
    MemFrame* clockPtr;
    MemFrame** frameList;// simple frame mapping
} RAM;

typedef struct{
    unsigned long replacedpid;
    unsigned long replacedvpn;
    int ppn;
} PageInfo;

RAM* initRAM(int numOfFrame);

PageInfo FIFO_LRURead(RAM* mem, unsigned long pid, unsigned long vpn);

PageInfo clockRead(RAM* mem, unsigned long pid, unsigned long vpn);

void LRURef(RAM* mem, int ppn);

void clockRef(RAM* mem, int ppn);

void cleanProcRAM(RAM* mem, unsigned long pid);

void recycleRAM(RAM* mem);

#endif
