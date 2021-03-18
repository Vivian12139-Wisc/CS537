//////////////////////////////////////////////////////////
//Main file: 537ps.c
//This file: printOps.c
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "printOps.h"

/*
 * This function gets all information needed and print them out
 *
 *@arg Info *infoList, all information possibly needed
 *@arg options *op, the request from user
 */
void printOption(Info *infoList, options* op) {
    if (infoList == NULL || op == NULL) {
        printUsage();
        return;
    }

    int procNum = op->pNum;

    for (int i = 0; i < procNum; i++) {
        printf("%d: ", op->pids[i]);
        if (op->sFlag == 1) 
            printf("%c ", infoList[i].state);
   
        if (op->UFlag == 1) 
            printf("utime=%d ", infoList[i].utime);
    
        if (op->SFlag == 1) 
            printf("stime=%d ", infoList[i].stime);

        if (op->vFlag == 1) 
            printf("size=%d ", infoList[i].size);
        
        
        if (op->cFlag == 1) {
            printf("[");
            for (int j = 0; j < infoList[i].cmdlength; j++)
                printf("%c",infoList[i].cmdline[j]);
            printf("]");
        }
        
        printf("\n");

    } 

    
}

