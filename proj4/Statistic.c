////////////////////////////////////////////
//Main file: 537pfsim-fifo.c, 537pfsim-lru.c, 537pfsim-clock.c
//This file: Statistic.c
//Semester: CS537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "Statistic.h"

/*
 * This function prints the statistic information of the simulator
 * @arg clock the total clock time
 * @arg tmu the total page frames are occupied
 * @arg trp the total number of processes that are runnable
 * @arg tmr the total total number of memroy references
 * @arg tpi the total number of page faults
 * @arg pageFramNum the total number of page frames
 */
void printStat(long clock, long long tmu, long long trp, long long tmr, long long tpi, int pageFrameNum) { 
    printf("AMU: %f\n", (float)tmu / (clock * pageFrameNum));     
    printf("ARP: %f\n", (float)trp / clock); 
    printf("TMR: %lld\n", tmr);
    printf("TPI: %lld\n", tpi);
    printf("RTime: %ld\n", clock);
}
