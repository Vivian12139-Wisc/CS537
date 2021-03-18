////////////////////////////////////////////
//Main file: 537pfsim-fifo.c, 537pfsim-lru.c, 537pfsim-clock.c
//This file: CollectInfo.h
//Semester: CS537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
////////////////////////////////////////////

#ifndef COLLECTINFO_H_INCLUDED
#define COLLECTINFO_H_INCLUDED

typedef struct{
    long start;// the start of a block
    long end;// the end of a block
} Block;

typedef struct{
    unsigned long* pids;
    Block** blocks;
    int* blockNum;// number of blocks for each process
    int pidNum;
} TraceFileInfo;

TraceFileInfo* collectInfo(FILE* tracefile);

void recycleInfo(TraceFileInfo* metadata);

#endif
