//////////////////////////////////////////////////////////
//Main file: 537ps.c
//This file: parseInfo.h
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////
#ifndef PARSEINFO_H_INCLUDED
#define PARSEINFO_H_INCLUDED

/*
 * The returning structure containing the information of a process
 */
typedef struct {
    char state;
    int utime, stime, size, cmdlength;
    char *cmdline;
} Info;

Info* parse(int pNum, int *pids);

#endif
