////////////////////////////////////////////
//Main file: 537pfsim-fifo.c, 537pfsim-lru.c, 537pfsim-clock.c
//This file: 537pfsim-lru.c
//Semester: CS537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
////////////////////////////////////////////

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "CollectInfo.h"
#include "Input.h"
#include "PageTable.h"
#include "RAM.h"
#include "PagingDevice.h"
#include "Statistic.h"

int main (int argc, char* argv[]){
    MemInfo inputInfo = input(argc, argv);
    FILE* tracefile;
    if ((tracefile = fopen(inputInfo.tracefile , "r")) == NULL){
        printf("Failed to open the tracefile\n");
        exit(EXIT_FAILURE);
    }
    // initialize all components
    TraceFileInfo* metadata = collectInfo(tracefile);
    HashPageTable* hashPageTables = initPageTable(inputInfo.frameNum, metadata);
    RAM* mem = initRAM(inputInfo.frameNum);
    PagingDevice* pagingDevice = initPagingDevice(metadata);
    long clock = 0;
    long curPos = ftell(tracefile);
    contextSwitch(pagingDevice, tracefile);
    unsigned long pid = (unsigned long)0;
    unsigned long vpn = (unsigned long)0;
    int ppn = 0;
    int curProc = pagingDevice->curProc;
    long long tmr = 0;
    long long tpi = 0;
    long prevClock = 0;
    long long tmu = 0; // total memory utilization
    long long trp = 0; // total runnable processes
    long long totalProcNum = metadata->pidNum; 
    while (1){
        curProc = pagingDevice->curProc;
        if (curProc != -1){
            // there's at least one runnable process
            fscanf(tracefile, "%lu %lu", &pid, &vpn);
            curPos = ftell(tracefile);
            if ((ppn = getPpn(hashPageTables, pid, vpn)) != -1){
                // successful memory reference
                // make a memory reference
                tmr ++;
                LRURef(mem, ppn);
                pagingDevice->readReqs[curProc]->nextLine = curPos;
                // if tracefile reached the end of a block, context switch
                if (curPos >= metadata->blocks[curProc][pagingDevice->readReqs[curProc]->curBlock].end){
                    // if this is the end of the last block, end this process
                    if (pagingDevice->readReqs[curProc]->curBlock == metadata->blockNum[curProc] - 1){
                        // the process has been ended
                        totalProcNum --;         
                        pagingDevice->readReqs[curProc]->procState = END;
                        cleanProcPT(hashPageTables, metadata->pids[curProc]);
                        cleanProcRAM(mem, metadata->pids[curProc]);
                        int end = 1;
                        for(int i = 0; i < metadata->pidNum; i++){
                            if (pagingDevice->readReqs[i]->procState != END){
                                end = 0;
                                break;
                            }
                        }
                        if (end){
                            clock++;
                            break;
                        }
                    }
                    else {
                        pagingDevice->readReqs[curProc]->curBlock++;
                        pagingDevice->readReqs[curProc]->nextLine = metadata->blocks[curProc][pagingDevice->readReqs[curProc]->curBlock].start;
                    }
                    contextSwitch(pagingDevice, tracefile);
                }
            } else {
                // failed memory reference
                tpi ++;
                readReq(pagingDevice, pid, vpn);
                contextSwitch(pagingDevice, tracefile);
            }
        } else {
            prevClock = clock; 
            // no runnable process, skip rest of the current I/O time
            clock = skipReadTime(pagingDevice, clock);
            tmu = tmu + (clock - prevClock) * mem->usedFrameNum;  
        }
        // let the disk update its state
        timeFlow(pagingDevice, mem, hashPageTables, tracefile, LRU);
        clock++;
        tmu += mem->usedFrameNum;
        trp = trp + (totalProcNum - pagingDevice->reqQueueNum);
    }
    // print statistics
    printStat(clock, tmu, trp, tmr, tpi, mem->numOfFrame);
    // free all heap space
    recycleAll(metadata, hashPageTables, mem, pagingDevice);
}
