////////////////////////////////////////////
//Main file: 537pfsim-fifo.c, 537pfsim-lru.c, 537pfsim-clock.c
//This file: Input.h
//Semester: CS537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
////////////////////////////////////////////

#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

typedef struct{
    int frameNum;// number of page frames
    char* tracefile;
} MemInfo;

int powerOfTwo(int input);

MemInfo input(int argc, char* argv[]);

void printUsage();

#endif
