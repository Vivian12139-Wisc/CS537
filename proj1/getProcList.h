//////////////////////////////////////////////
//Main file: 537ps.c
//This file: getProcList.h
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//Email: xye53@wisc.edu, czhu226@wisc.edu
/////////////////////////////////////////////
#ifndef GETPROCLIST_H_INCLUDED
#define GETPROCLIST_H_INCLUDED

/*
 * The returning structure containing the number of process and their ids
 */
typedef struct {
    int pnum;
    int *pids;
}procList;

procList* getProcList();

#endif
