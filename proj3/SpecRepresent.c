//////////////////////////////////////////////////////////
//Main file: 537make.c
//This file: SpecRepresent.c
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//NetID: xye53, czhu226
//CompSci Login: xunwei, chenjie
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <fcntl.h>
#include <string.h>
#include "SpecRepresent.h"
#include "Stack.h"

/*
 * This function run outdated rules in the reverse order of topological sort
 *
 * @param label, the label to be runned 
 * @param data, the data of makefile
 */
void runRules(char *label, metadata data) {
    int* visited;
    int* topoSort;
    int* outdated;
    int* runOrder;
    int orderNum = data.fileNum;
    stack* st = createStack(data.fileNum);
    
    if ((outdated = calloc(data.fileNum, sizeof(int))) == NULL) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    if ((topoSort = calloc(data.fileNum, sizeof(int))) == NULL) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    if ((visited = calloc(data.fileNum, sizeof(int))) == NULL) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    
    int labelIndex = -1;
    for (int i = 0; i < data.fileNum; i ++) {
        if (strcmp(data.files[i].name, label) == 0) 
            labelIndex = i;
    }
    // Cannot find the label   
    if (labelIndex == -1) {
        printf("The label does not exist in the makefile!\n");
        exit(EXIT_FAILURE);      
    }

    visited[labelIndex] = 1;
    push(labelIndex, st);
  
    while (!isEmpty(st)) {
        int cur = peek(st);
        int unvisitedSucc = -1;

        for (int i = 0; i < data.files[cur].depNum; i ++) {
            if (visited[data.files[cur].dependency[i]] == 0) {
                unvisitedSucc = i;
                break;
            }
        }

        if (unvisitedSucc == -1) {
            cur = pop(st);
            topoSort[cur] = orderNum;
            orderNum --;
        } else {
            visited[data.files[cur].dependency[unvisitedSucc]] = 1;
            push(data.files[cur].dependency[unvisitedSucc], st);
        }
    }
    
    orderNum = data.fileNum;
    if ((runOrder = calloc(data.fileNum, sizeof(int))) == NULL) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < data.fileNum; i ++) 
        runOrder[i] = -1;

    while (orderNum > 0) {
        for (int i = 0; i < data.fileNum; i ++) {
            if (topoSort[i] == orderNum)  
                runOrder[orderNum - 1] = i;
        }
        orderNum --; 
    }

    for (int i = data.fileNum - 1; i >= 0; i --) {
        if (runOrder[i] != -1 && data.files[runOrder[i]].isTarget == 1) {
            if (data.files[runOrder[i]].depNum == 0) {
                outdated[runOrder[i]] = 1;
            } else {
                // makeRule 12 
                for (int j = 0; j < data.files[runOrder[i]].depNum; j ++) {
                    int dep = data.files[runOrder[i]].dependency[j];
                    FILE *fp;
                    if ((fp = fopen(data.files[dep].name, "r")) != NULL) {
                        if (data.modifyTime[dep] > data.modifyTime[runOrder[i]]) 
                            outdated[runOrder[i]] = 1;
                        fclose(fp);
                    } 
                    if (outdated[dep] == 1)
                        outdated[runOrder[i]] = 1;
                }  
            }
        }
    }

    for (int i = data.fileNum - 1; i >= 0; i --) {
        int run = runOrder[i];
        if (run != -1 && data.files[run].isTarget == 1 && outdated[run] == 1) { 
            for (int j = 0; j < data.files[run].cmdNum; j ++)  
                if (runCmd(data.files[run].cmd[j]) != 0) {
                    printf("Command execute error1!\n");
                    exit(EXIT_FAILURE);
                }
        }
    }    
    
    free(visited);
    free(topoSort);
    free(outdated);
    free(runOrder);
    free(st->array);
    free(st);  
}

