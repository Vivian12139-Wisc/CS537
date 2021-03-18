//////////////////////////////////////////////////////////
//Main file: 537ps.c
//This file: parseInfo.c
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include "parseInfo.h"


/*
 * This function returns all possibly needed information about input processes
 *
 * @arg int pNum, the number of requested processes
 * @arg int* pids, the pointer to all requested pids
 * @return Info*, an array of Info struct containing information
 */
Info* parse(int pNum, int* pids) {
    if (pNum == 0 || pids == NULL) 
        return NULL;

    const int strLength = 50;
    const int cmdBuffer = 200;
    Info *infoList;
    infoList = malloc(sizeof(Info) * pNum);

    if (infoList == NULL) {
        printf("Can't allocate memory for infoList!\n");
        exit(EXIT_FAILURE);
    }
       
    for (int i = 0; i < pNum; i++) {
        FILE *fp1; //stat file
        FILE *fp2; //statm file
        FILE *fp3; //file

        char pidStr[strLength];        
        char fileStr1[strLength];
        char fileStr2[strLength];
        char fileStr3[strLength];
        
        // make up the file names
        sprintf(pidStr, "%d", pids[i]);
        sprintf(fileStr1, "/proc/%d/stat", pids[i]);
        sprintf(fileStr2, "/proc/%d/statm", pids[i]);
        sprintf(fileStr3, "/proc/%d/cmdline", pids[i]);

        fp1 = fopen(fileStr1, "r");
        fp2 = fopen(fileStr2, "r");
        fp3 = fopen(fileStr3, "r");
        
        if (fp1 == NULL) {
            printf("Can't open file stat for pid %d!\n", pids[i]);
            exit(EXIT_FAILURE);
        } 

        if (fp2 == NULL) {
            printf("Can't open file statm for pid %d!\n", pids[i]);
            exit(EXIT_FAILURE);
        } 

        if (fp3 == NULL) {
            printf("Can't open file cmdline for pid %d!\n", pids[i]);
            exit(EXIT_FAILURE);
        } 
        
        //scan data at certain position
        if (fscanf(fp1, "%*d %*s %c %*d %*d %*d %*d %*d %*u %*u %*d %*d %*d %d %d", &(infoList[i].state), &(infoList[i].utime), &(infoList[i].stime)) != 3) {
            printf("Read stat file error for pid %d!\n", pids[i]);
            exit(EXIT_FAILURE);
        }
        
        if (fscanf(fp2, "%i", &(infoList[i].size)) != 1) {
            printf("Read statm file error for pid %d!\n", pids[i]);
            exit(EXIT_FAILURE);
        }
        
        infoList[i].cmdline = (char*)malloc(sizeof(char) * cmdBuffer);
        if (infoList[i].cmdline == NULL) {
            puts("Can't allocate memory for cmdline\n");
            exit(EXIT_FAILURE);
        }
        //scan cmdline char by char until the end of file in case encountering null
        int c;
        int cmdPosition = 0;
        do {
            if ((c = fgetc(fp3)) == EOF) {
                if (feof(fp3))
                    break;
                else {
                    printf("Scanning error\n");
                    exit(EXIT_FAILURE);
                }
            }
            infoList[i].cmdline[cmdPosition] = (char) c;
            cmdPosition++;
        } while(1);
        infoList[i].cmdlength = cmdPosition;

        if (fclose(fp1) != 0) {
            printf("Can't close stat file for pid %d!\n", pids[i]);
            exit(EXIT_FAILURE);
        }
        
        if (fclose(fp2) != 0) {
            printf("Can't close statm file for pid %d!\n", pids[i]);
            exit(EXIT_FAILURE);
        } 
        if (fclose(fp3) != 0) {
            puts("Can't close cmdline file for pid %d!\n");
            exit(EXIT_FAILURE);
        } 
    }
     
    return infoList;
}

        
