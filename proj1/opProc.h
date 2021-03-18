/////////////////////////////////////////
//Main file: 537ps.c
//This file: opProc.h
//Semester: CS537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
/////////////////////////////////////////
#ifndef OPPROC_H_INCLUDED
#define OPPROC_H_INCLUDED

/*
 * This struct include all information input by the user
 */
typedef struct {
    int pNum, sFlag, UFlag, SFlag, vFlag, cFlag;
    int *pids;
} options;

options* opProc(int argc, char *argv[]);

void printUsage();

#endif
