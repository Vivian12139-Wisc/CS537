//////////////////////////////////////////////////////////
//Main file: 537make.c
//This file: Stack.c
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//NetID: xye53, czhu226
//CompSci Login: xunwei, chenjie
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"

/*
 * This function checks if the stack is empty
 *
 * @param st, the pointer of a stack
 * @return 1 if the stack is empty, and 0 otherwise
 */
int isEmpty(stack *st) {
    return ((st->elemNum == 0) ? 1 : 0);
}

/*
 * This function initializes a stack 
 *
 * @param capacity, the largest capacity of a stack
 * @returna pointer of a newly created stack
 */
stack* createStack(int capacity) {
    stack *st;
    if ((st = malloc(sizeof(stack))) == NULL) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
   
    st->stackTop = -1;  
    st->elemNum = 0;
    if ((st->array = malloc(sizeof(int) * capacity)) == NULL) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    return st;   
}

/*
 * This function push a element into the stack
 *
 * @param elem, the element to be pushed into the stack
 * @param st, the pointer of a stack
 */
void push(int elem, stack *st) {
    st->stackTop ++;
    st->elemNum ++;
    st->array[st->stackTop] = elem;
}

/*
 * This function peek the element of the stack
 *
 * @param st, the pointer of a stack
 * @return the element on the top of the stack
 */  
int peek(stack *st) {
    return st->array[st->stackTop];
}  

/*
 * This function pop the element of the stack
 *
 * @param st, the pointer of a stack
 * @return the element on the top of the stack
 */
int pop(stack *st) {
    st->elemNum --; 
    return st->array[st->stackTop--];
}    
