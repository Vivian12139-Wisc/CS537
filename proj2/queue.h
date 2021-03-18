//////////////////////////////////////////////////////////
//Main file: main.c
//This file: queue.h
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//NetID: xye53, czhu226
//CompSci Login: xunwei, chenjie
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////

#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <semaphore.h>

/*
 * This struct contains an array of char* as a queue and other queue statistics
 */
typedef struct{
    char **stringQueue;
    int enqueueCount, dequeueCount, strings, size;
    float enqueueTime, dequeueTime;
    sem_t full, empty, mutex;
} Queue;

Queue *CreateStringQueue(int size);
void EnqueueString(Queue *q, char* string);
char *DequeueString(Queue *q);
void PrintQueueStats(Queue *q);

#endif
