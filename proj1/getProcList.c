//////////////////////////////////////////////////////////
//Main file: 537ps.c
//This file: getProcList.c
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "getProcList.h"

/*
 * This function returns all process ids which belong to the current user
 * 
 * @arg none
 * @return struct procList
 */
procList* getProcList(){
    DIR *proc;
    FILE *file;
    struct dirent *subdir;
    procList *pidList;
    int pnum, pid, uid;
    long loginuid;
    const char *dir = "/proc";
    pnum = 0;
    //Get current uid
    uid = getuid();
    //Memory allocation for pidList and pids
    if (((pidList = malloc(sizeof(procList))) == NULL) || ((pidList -> pids = malloc(sizeof(int))) == NULL)) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(-1);
    }
    //Open /proc directory
    proc = opendir(dir);
    if (proc) {
        while ((subdir = readdir(proc))) {
            //Get the next pid
            pid = atoi(subdir -> d_name);
            if (pid != 0) {
                char *path;
                if ((path = malloc(strlen("/proc/") + strlen(subdir -> d_name))) == NULL) {
                    fprintf(stderr, "Memory allocation failed.\n");
                    exit(-1);
                }
                //Construct the path of loginuid file
                strcpy(path, "/proc/");
                strcat(path, subdir -> d_name);
                strcat(path, "/loginuid");
                //Read the loginuid file and get the uid of this process
                if ((file = fopen(path, "r")) == NULL) {
                    fprintf(stderr, "Failed to open the file.\n");
                    exit(-1);
                }
                if (!(fscanf(file, "%ld", &loginuid))) {
                    fprintf(stderr, "Failed to read the file.\n");
                    exit(-1);
                }
                //If this process belongs to the current user, add pid to the list
                if (loginuid == (long)uid) {
                    if (pnum > 0) {
                        if ((pidList -> pids = realloc(pidList -> pids, sizeof(int) * (pnum + 1))) == NULL) {
                            fprintf(stderr, "Memory allocation failed.\n");
                            exit(-1);
                        }
                    }
                    (pidList -> pids)[pnum] = pid;
                    pnum++;
                }
                fclose(file);
            }
        }
        if (closedir(proc) != 0){
            fprintf(stderr, "Failed to close the directory.\n");
            exit(-1);
        }
    } else {
        fprintf(stderr, "Failed to open the directory.\n");
        exit(-1);
    }
    pidList -> pnum = pnum;
    return pidList;
}
