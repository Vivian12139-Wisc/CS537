//////////////////////////////////////////////////////////
//Main file: 537make.c
//This file: Run.c
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//NetID: xye53, czhu226
//CompSci Login: xunwei, chenjie
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <string.h>
#include "Run.h"

/*
 * This function runs the command using fork and execvp pairsh
 *
 * @param cmd, a command line represented as an array of strings 
 */
int runCmd(char** cmd) {
    int rc = fork();
    int wstatus;
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        if (execvp(cmd[0], cmd)) {
            printf("Cannot run command!\n");
            exit(EXIT_FAILURE);
        }
    } else {
        if (wait(&wstatus) < 0) {
            printf("Command execute error!\n");
            exit(EXIT_FAILURE);
        }
    }
    return WEXITSTATUS(wstatus);
}


