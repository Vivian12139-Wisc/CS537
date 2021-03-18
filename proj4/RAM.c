////////////////////////////////////////////
//Main file: 537pfsim-fifo.c, 537pfsim-lru.c, 537pfsim-clock.c
//This file: RAM.c
//Semester: CS537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
////////////////////////////////////////////

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "RAM.h"
/*
 * This function initializes the memory
 * @arg the numbee of frames of RAM
 * @return the RAM that has been initialized
 */
RAM* initRAM(int numOfFrame){
    RAM* mem;
    if ((mem = malloc(sizeof(RAM))) == NULL ||
    (mem->frameList = malloc(sizeof(MemFrame*) * numOfFrame)) == NULL){
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    mem->numOfFrame = numOfFrame;
    mem->usedFrames = NULL;
    mem->usedFrameNum = 0;
    MemFrame** frameEntry = &(mem->freeFrames);
    MemFrame* lastFrame = NULL;
    for (int i = 0; i < numOfFrame; i++){
        if ((*frameEntry = malloc(sizeof(MemFrame))) == NULL){
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        mem->frameList[i] = *frameEntry;
        (*frameEntry)->ppn = i;
        (*frameEntry)->refbit = 0;
        (*frameEntry)->pid = (unsigned long) 0;
        (*frameEntry)->vpn = (unsigned long) 0;
        (*frameEntry)->nextFrame = NULL;
        (*frameEntry)->lastFrame = lastFrame;
        lastFrame = *frameEntry;
        if (i < numOfFrame - 1)
            frameEntry = &((*frameEntry)->nextFrame);
    }
    (*frameEntry)->nextFrame = mem->freeFrames;
    mem->freeFrames->lastFrame = *frameEntry;
    mem->clockPtr = mem->freeFrames;
    return mem;
}

/*
 * This function uses FIFO and LRU policy to replace a memory page
 * It returns the replaced (pid, vpn) and the ppn selected
 * return pid=0 and vpn=0 if there are free frames available
 */
PageInfo FIFO_LRURead(RAM* mem, unsigned long pid, unsigned long vpn){
    MemFrame* targetFrame;
    PageInfo pageInfo;
    if (mem->freeFrames == NULL){
        targetFrame = mem->usedFrames;
        mem->usedFrames = targetFrame->nextFrame;
        pageInfo.replacedpid = targetFrame->pid;
        pageInfo.replacedvpn = targetFrame->vpn;
        pageInfo.ppn = targetFrame->ppn;
        targetFrame->pid = pid;
        targetFrame->vpn = vpn;
        targetFrame->refbit = 1;
    } else {
        targetFrame = mem->freeFrames;
        if (targetFrame->nextFrame == targetFrame)
            mem->freeFrames = NULL;
        else {
            mem->freeFrames = targetFrame->nextFrame;
            mem->freeFrames->lastFrame = targetFrame->lastFrame;
            targetFrame->lastFrame->nextFrame = mem->freeFrames;
        }
        pageInfo.replacedpid = targetFrame->pid;
        pageInfo.replacedvpn = targetFrame->vpn;
        pageInfo.ppn = targetFrame->ppn;
        targetFrame->pid = pid;
        targetFrame->vpn = vpn;
        targetFrame->refbit = 1;
        targetFrame->nextFrame = NULL;
        if (mem->usedFrames == NULL){
            mem->usedFrames = targetFrame;
            targetFrame->nextFrame = targetFrame;
            targetFrame->lastFrame = targetFrame;
        } else {
            mem->usedFrames->lastFrame->nextFrame = targetFrame;
            targetFrame->lastFrame = mem->usedFrames->lastFrame;
            targetFrame->nextFrame = mem->usedFrames;
            mem->usedFrames->lastFrame = targetFrame;
        }
        // there is free frames in RAM
        mem->usedFrameNum ++;
    }
    return pageInfo;
}

/*
 * This function uses clock algorithm to replace a memory page
 * It returns the replaced (pid, vpn) and the ppn selected
 * return pid=0 and vpn=0 if there are free frames available
 */
PageInfo clockRead(RAM* mem, unsigned long pid, unsigned long vpn){
    MemFrame* targetFrame;
    PageInfo pageInfo;
    if (mem->freeFrames == NULL){
        while (1){
            if (mem->clockPtr->refbit == 1){
                mem->clockPtr->refbit = 0;
                mem->clockPtr = mem->frameList[(mem->clockPtr->ppn + 1) % mem->numOfFrame]; 
            } else {
                targetFrame = mem->clockPtr;
                pageInfo.replacedpid = targetFrame->pid;
                pageInfo.replacedvpn = targetFrame->vpn;
                pageInfo.ppn = targetFrame->ppn;
                targetFrame->pid = pid;
                targetFrame->vpn = vpn;
                targetFrame->refbit = 0;
                mem->clockPtr = mem->frameList[(targetFrame->ppn + 1) % mem->numOfFrame];
                break;
            }
        }
    } else {
        while (1){
            if (mem->clockPtr->pid == (unsigned long) 0 && mem->clockPtr->vpn == (unsigned long) 0){
                targetFrame = mem->clockPtr;
                mem->clockPtr = mem->frameList[(mem->clockPtr->ppn + 1) % mem->numOfFrame];
                break;
            } else 
                mem->clockPtr = mem->frameList[(mem->clockPtr->ppn + 1) % mem->numOfFrame];
        }
        if (targetFrame->nextFrame == targetFrame)
            mem->freeFrames = NULL;
        else {
            if (mem->freeFrames == targetFrame)
                mem->freeFrames = targetFrame->nextFrame;
            targetFrame->nextFrame->lastFrame = targetFrame->lastFrame;
            targetFrame->lastFrame->nextFrame = targetFrame->nextFrame;
        }
        pageInfo.replacedpid = targetFrame->pid;
        pageInfo.replacedvpn = targetFrame->vpn;
        pageInfo.ppn = targetFrame->ppn;
        targetFrame->pid = pid;
        targetFrame->vpn = vpn;
        targetFrame->refbit = 0;//here
        targetFrame->nextFrame = NULL;
        if (mem->usedFrames == NULL){
            mem->usedFrames = targetFrame;
            targetFrame->nextFrame = targetFrame;
            targetFrame->lastFrame = targetFrame;
        } else {
            mem->usedFrames->lastFrame->nextFrame = targetFrame;
            targetFrame->lastFrame = mem->usedFrames->lastFrame;
            targetFrame->nextFrame = mem->usedFrames;
            mem->usedFrames->lastFrame = targetFrame;
        }
        // there is free frame in RAM
        mem->usedFrameNum ++;
    }
    return pageInfo;
}

/*
 * This function makes a memory reference using LRU policy
 */
void LRURef(RAM* mem, int ppn){
    MemFrame* targetFrame = mem->frameList[ppn];
    if (mem->usedFrames == targetFrame)
        mem->usedFrames = targetFrame->nextFrame;
    targetFrame->lastFrame->nextFrame = targetFrame->nextFrame;
    targetFrame->nextFrame->lastFrame = targetFrame->lastFrame;
    mem->usedFrames->lastFrame->nextFrame = targetFrame;
    targetFrame->lastFrame = mem->usedFrames->lastFrame;
    targetFrame->nextFrame = mem->usedFrames;
    mem->usedFrames->lastFrame = targetFrame;
    targetFrame->refbit = 1;
}

/*
 * This function makes a memory reference using clock algorithm policy
 */
void clockRef(RAM* mem, int ppn){
    mem->frameList[ppn]->refbit = 1;
}

/*
 * This function cleans memory space for a certain process
 */
void cleanProcRAM(RAM* mem, unsigned long pid){
    MemFrame* targetFrame;
    for (int i = 0; i < mem->numOfFrame; i++){
        targetFrame = mem->frameList[i];
        if (targetFrame->pid == pid){
            if (mem->usedFrames == targetFrame){
                if (targetFrame->nextFrame == targetFrame)
                    mem->usedFrames = NULL;
                else
                    mem->usedFrames = targetFrame->nextFrame;
            }
            targetFrame->lastFrame->nextFrame = targetFrame->nextFrame;
            targetFrame->nextFrame->lastFrame = targetFrame->lastFrame;
            targetFrame->pid = (unsigned long) 0;
            targetFrame->vpn = (unsigned long) 0;
            targetFrame->refbit = 0;
            if (mem->freeFrames == NULL){
                mem->freeFrames = targetFrame;
                targetFrame->lastFrame = targetFrame;
                targetFrame->nextFrame = targetFrame;
            } else {
                mem->freeFrames->lastFrame->nextFrame = targetFrame;
                targetFrame->lastFrame = mem->freeFrames->lastFrame;
                targetFrame->nextFrame = mem->freeFrames;
                mem->freeFrames->lastFrame = targetFrame;
            }
            mem->usedFrameNum --; 
        }
    }
}

/*
 * this function recycles all memory space for RAM
 */
void recycleRAM(RAM* mem){
    for (int i = 0; i < mem->numOfFrame; i++)
        free(mem->frameList[i]);
    free(mem);
}
