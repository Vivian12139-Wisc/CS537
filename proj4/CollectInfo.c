////////////////////////////////////////////
//Main file: 537pfsim-fifo.c, 537pfsim-lru.c, 537pfsim-clock.c
//This file: CollectInfo.c
//Semester: CS537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "CollectInfo.h"

const int BUFFER_SIZE = 4096;

/*
 * This function read the tracefile for the first time
 * It detects potential error and marks the start and end of each process trace block
 * @arg tracefile the file to be read
 * @return the trace file information that has been collected
 */
TraceFileInfo* collectInfo(FILE* tracefile){
    TraceFileInfo* metadata;
    if((metadata = malloc(sizeof(TraceFileInfo))) == NULL || 
    (metadata->pids = malloc(sizeof(unsigned long))) == NULL || 
    (metadata->blockNum = malloc(sizeof(int))) == NULL ||
    (metadata->blocks = malloc(sizeof(Block*))) == NULL){
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    metadata->pidNum = 0;
    metadata->pids[0] = 0;
    metadata->blockNum[0] = 0;
    metadata->blocks[0] = NULL;
    unsigned long pid = (unsigned long)0;
    unsigned long lastpid = (unsigned long)0;
    char pidStr[BUFFER_SIZE];
    char vpnStr[BUFFER_SIZE];
    char *strPtr;
    int procNum = -1;
    int blockNum = -1;
    long position = ftell(tracefile);
    while(fscanf(tracefile, "%s %s", pidStr, vpnStr) > 0){
        // examine if there's any incorrect input
        pid = strtoul(pidStr, &strPtr, 10);
        if ((int)strlen(strPtr)){
            printf("Error found in the tracefile\n");
            exit(EXIT_FAILURE);
        }
        strtoul(vpnStr, &strPtr, 10);
        if ((int)strlen(strPtr)){
            printf("Error found in the tracefile\n");
            exit(EXIT_FAILURE);
        }
        // if a new block was detected, mark current position as the end of current block
        if (pid != lastpid){
            if (procNum != -1)
                metadata->blocks[procNum][metadata->blockNum[procNum] - 1].end = position;
            procNum = -1;
            for (int i = 0; i < metadata->pidNum; i++){
                if (metadata->pids[i] == pid){
                    procNum = i;
                    break;
                }
            }
            if (procNum == -1){
                procNum = metadata->pidNum++;
                if ((metadata->pids = realloc(metadata->pids, sizeof(unsigned long) * metadata->pidNum)) == NULL ||
                (metadata->blocks = realloc(metadata->blocks, sizeof(Block*) * metadata->pidNum)) == NULL ||
                (metadata->blockNum = realloc(metadata->blockNum, sizeof(int) * metadata->pidNum)) == NULL){
                    printf("Memory allocation failed\n");
                    exit(EXIT_FAILURE);
                }
                metadata->pids[procNum] = pid;
                metadata->blockNum[procNum] = 0;
            }
            blockNum = metadata->blockNum[procNum]++;
            if ((metadata->blocks[procNum] = realloc(metadata->blocks[procNum], sizeof(Block) * metadata->blockNum[procNum])) == NULL){
                printf("Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            metadata->blocks[procNum][blockNum].start = position;
            lastpid = pid;
        }
        position = ftell(tracefile);
    }
    metadata->blocks[procNum][blockNum].end = position;
    return metadata;        
}

/*
 * This function recycles the space of TraceFileInfo
 * @metadata the metadata of tracefile to be freed
 */
void recycleInfo(TraceFileInfo* metadata){
    for (int i = 0 ; i < metadata->pidNum; i++)
        free(metadata->blocks[i]);
    free(metadata->pids);
    free(metadata->blocks);
    free(metadata->blockNum);
    free(metadata);
}
