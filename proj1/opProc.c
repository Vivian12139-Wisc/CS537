////////////////////////////////////////////
//Main file: 537ps.c
//This file: opProc.c
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
#include "opProc.h"

/*
 * This function processes the flags and arguments input by the user
 *
 * @arg int argc, the amount of argument
 * @arg char* argv[] arguments input by the user
 * @return options* info, the pointer to a struct storing all information
 */
options* opProc(int argc, char *argv[]){
    int opt;
    options *info;
    
    if ((info = malloc(sizeof(options))) == NULL) {
        fprintf(stderr, "%s\n", "Memory allocation failed.");
        exit(-1);
    }
    //All flags are set to its default preliminary value
    const char *dash = "-";
    int sFlag = 0;
    int UFlag = 1;
    int SFlag = 0;
    int vFlag = 0;
    int cFlag = 1;
    int pNum = 0;
    int pVal;
    
    while ((opt = getopt(argc, argv, "p:s::U::S::v::c::m:")) != -1) {
        switch (opt) {
            //Process -p flag here
            case 'p':
                pVal = atoi(optarg);
                if (pVal == 0)
                    printUsage();

                if (pNum == 0) {
                    if ((info -> pids = malloc(sizeof(int))) == NULL) {
                        fprintf(stderr, "%s\n", "Memory allocation failed.");
                        exit(-1);
                    }
                } else {
                    if ((info -> pids = realloc(info -> pids, sizeof(int) * (pNum + 1))) == NULL) {
                        fprintf(stderr, "%s\n", "Memory allocation failed.");
                        exit(-1);
                    }
                }
                (info -> pids)[pNum] = pVal;
                pNum++;
                break;

            //Process -s flag here
            case 's':
                if (optarg == NULL) {
                    sFlag = 1;
                    break;
				}
                                
                if (strcmp(optarg, dash) == 0) {
                    sFlag = 0;
                    break;
                }
                else
                    printUsage();

            //Process -U flag here
            case 'U':
                if(optarg == NULL){
                    UFlag = 1;
                    break;
                }
                if(strcmp(optarg, dash) == 0) {
                    UFlag = 0;
                    break;
                }
                else
                    printUsage();

            //Process -S flag here
            case 'S':
                if (optarg == NULL) {
                    SFlag = 1;
                    break;
                }

                if (strcmp(optarg, dash) == 0) {
                    SFlag = 0;
                    break;
                }
                else
                    printUsage();

            //Process -v flag here
            case 'v':
                if (optarg == NULL){
                    vFlag = 1;
                    break;
                }
				
                if (strcmp(optarg, dash) == 0) {
                    vFlag = 0;
                    break;
                }
                else
                    printUsage();
            
            //Process -c flag here
            case 'c':
                if (optarg == NULL) {
                    cFlag = 1;
                    break;
                }
                if (strcmp(optarg, dash) == 0) {
                    cFlag = 0;
                    break;
                }
                else
                    printUsage();

            //Process -m flag here
            case 'm':
                printf("-m option not supportive\n");
                break;
            
            //Process all other wrong input here
            default:
                printUsage();
                break;
        }
    }

    info -> pNum = pNum;    
    info -> sFlag = sFlag;
    info -> UFlag = UFlag;
    info -> SFlag = SFlag;
    info -> vFlag = vFlag;
    info -> cFlag = cFlag;

    return info;
}

/*
 * This function prints usage and exit when an error occurs
 */
void printUsage()   {
    printf("Wrong arguments. Usage: 537ps [-p <pid>] [-s] [-U] [-S] [-v] [-c] [-m <addr> <len>]\n");
    exit(EXIT_FAILURE);
}
