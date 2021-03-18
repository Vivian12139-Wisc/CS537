//////////////////////////////////////////////////////////
//Main file: main.c
//This file: queue.c
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//NetID: xye53, czhu226
//CompSci Login: xunwei, chenjie
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////

#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include "queue.h"

/*
 * This function creates a new string queue of certain size, initializes all statistics
 *
 * @arg int size
 * @return Queue* newQueue
 */
Queue *CreateStringQueue(int size){
    Queue *newQueue;
    if ((newQueue = malloc(sizeof(Queue))) == NULL) {
        printf("Failed to allocate memory to a new Queue.\n");
        exit(EXIT_FAILURE);
    }
    if ((newQueue -> stringQueue = malloc(sizeof(char*) * size)) == NULL) {
        printf("Failed to allocate memory to a new Queue.\n");
        exit(EXIT_FAILURE);
    }
    newQueue -> enqueueCount = 0;
    newQueue -> dequeueCount = 0;
    newQueue -> strings = 0;
    newQueue -> size = size;
    newQueue -> enqueueTime = 0.0;
    newQueue -> dequeueTime = 0.0;
    if (sem_init(&newQueue -> full, 0, 0) == -1) {
        printf("Failed to initialize semaphore.\n");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&newQueue -> empty, 0, size) == -1) {
        printf("Failed to initialize semaphore.\n");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&newQueue -> mutex, 0, 1) == -1) {
        printf("Failed to initialize semaphore.\n");
        exit(EXIT_FAILURE);
    }
    return newQueue;
}

/*
 * This function put a char* at the end of the string queue
 *
 * @arg Queue* q
 * @arg char* string
 */
void EnqueueString(Queue *q, char* string){
    struct timeval start, end;
    sem_wait(&q -> mutex);
    gettimeofday(&start, NULL);
    if (q -> strings == q -> size) {
        sem_post(&q -> mutex);
        sem_wait(&q -> empty);
        sem_wait(&q -> mutex);
    } else {
        sem_wait(&q -> empty);
    }
    (q -> stringQueue)[q -> strings] = string;
    q -> strings++;
    q -> enqueueCount++;
    sem_post(&q -> full);
    gettimeofday(&end, NULL);
    q -> enqueueTime += (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    sem_post(&q -> mutex);
}

/*
 * This function removes a char* from the beginning of queue and return the char*
 *
 * @arg Queue* q
 * @return char* temp
 */
char *DequeueString(Queue *q){
    struct timeval start, end;
    sem_wait(&q -> mutex);
    gettimeofday(&start, NULL);
    if (q -> strings == 0) {
        sem_post(&q -> mutex);
        sem_wait(&q -> full);
        sem_wait(&q -> mutex);
    } else {
        sem_wait(&q -> full);
    }
    char *temp = (q -> stringQueue)[0];
    for (int i = 0; i < q -> strings - 1; i++)
        (q -> stringQueue)[i] = (q -> stringQueue)[i+1];
    q -> strings--;
    q -> dequeueCount++;
    sem_post(&q -> empty);
    gettimeofday(&end, NULL);
    q -> dequeueTime += (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    sem_post(&q -> mutex);
    return temp;
}

/*
 * This function prints the statistics for this queue
 *
 * @arg Queue* q
 */
void PrintQueueStats(Queue *q){
    fprintf(stderr, "enqueueCount = %i\n", q -> enqueueCount);
    fprintf(stderr, "dequeueCount = %i\n", q -> dequeueCount);
    fprintf(stderr, "enqueueTime = %f\n", q -> enqueueTime);
    fprintf(stderr, "dequeueTime = %f\n", q -> dequeueTime);
}

