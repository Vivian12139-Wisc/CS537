//////////////////////////////////////////////////////////
//Main file: 537ps.c
//This file: 537ps.c
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "printOps.h"
#include "opProc.h"
#include "parseInfo.h"
#include "getProcList.h"

int main(int argc, char* argv[]) {
    if (argc < 0 || argv == NULL) {
        printUsage();
        exit(EXIT_FAILURE);
    }
    
    options *op = opProc(argc, argv);
    Info *infoList;
    
    if (op->pNum == 0) {
        procList* processes = getProcList();
        op -> pids = processes -> pids;
        op -> pNum = processes -> pnum;
        free(processes);
    } 
   
    infoList = parse(op -> pNum, op -> pids);
    
    printOption(infoList, op);

    free(op -> pids);
    for (int i = 0; i < op -> pNum; i++)
        free(infoList[i].cmdline);
    free(infoList);
    free(op);    
    
    return 0;
}
