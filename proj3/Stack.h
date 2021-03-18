//////////////////////////////////////////////////////////
//Main file: 537make.c
//This file: Stack.h
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//NetID: xye53, czhu226
//CompSci Login: xunwei, chenjie
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////

#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

// a stack data structure
typedef struct {
    int elemNum; // the current number of elements in the stack
    int stackTop; // the index of top of the stack
    int* array; // an array to represent stack
} stack;

int isEmpty(stack* st);
stack* createStack(int capacity);
void push(int elem, stack* st);
int peek(stack* st);
int pop(stack* st);

#endif
