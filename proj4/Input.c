////////////////////////////////////////////
//Main file: 537pfsim-fifo.c, 537pfsim-lru.c, 537pfsim-clock.c
//This file: Input.c
//Semester: CS537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
////////////////////////////////////////////
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "Input.h"

const int DEFAULT_PAGE_SIZE = 4096;
const int DEFAULT_MEM_SIZE = 1;
const int K = 1024;

/*
 * This function examines if a number is a power of two
 * @arg input the number to be examined
 * @return 1 if a number is a power of two, and 0 otherwise
 */
int powOfTwo(int input){
    int count = 0;
    while (input){
        count++;
        input = input & (input - 1);
    }
    if (count == 1)
        return 1;
    else
        return 0;
}

/*
 * This function processes the flags and arguments input by the user
 * @arg argc the count of command line arguments to be processed
 * @arg argv the array of command line arguments to be processed
 * @MemInfo the information of RAM to be intialized
 */
MemInfo input(int argc, char *argv[]){
    int opt;
    MemInfo info;
    info.tracefile = NULL;
    
    int pageSize = DEFAULT_PAGE_SIZE;
    int memSize = DEFAULT_MEM_SIZE;
    
    while ((opt = getopt(argc, argv, "p:m:")) != -1) {
        switch (opt) {
            //Process -p flag here
            case 'p':
                pageSize = atoi(optarg);
                if (pageSize == 0)
                    printUsage();
                if (!powOfTwo(pageSize)){
                    printf("Page size must be a power of two\n");
                    exit(EXIT_FAILURE);
                }
                break;

            //Process -m flag here
            case 'm':
                memSize = atoi(optarg);
                if (memSize == 0)
                    printUsage();
                break;

            //Process all other wrong input here
            default:
                printUsage();
                break;
        }
    }
    info.frameNum = memSize * K * K / pageSize;
    for(int i = optind; i < argc; i++){
        if (info.tracefile == NULL)
            info.tracefile = argv[i];
        else{
            printf("You can only input one name for the tracefile\n");
            exit(EXIT_FAILURE);
        }
    }
    if (info.tracefile == NULL)
        printUsage();
    return info;
}

/*
 * This function prints usage and exit when an error occurs
 */
void printUsage(){
    printf("Wrong arguments. Usage: 537pfsim[-fifo][-lru][-clock] [-p <pageSize>] [-m <memSize>] traceFileName \n");
    exit(EXIT_FAILURE);
}

