//////////////////////////////////////////////////////////
//Main file: main.c
//This file: thread.h
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//NetID: xye53, czhu226
//CompSci Login: xunwei, chenjie
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////

#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

void *Reader(void *arg);
void *Munch1(void *arg);
void *Munch2(void *arg);
void *Writer(void *arg);
int getLine(char* line, int size);

#endif
