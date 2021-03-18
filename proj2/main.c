//////////////////////////////////////////////////////////
//Main file: main.c
//This file: main.c
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//NetID: xye53, czhu226
//CompSci Login: xunwei, chenjie
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "queue.h"
#include "thread.h"

/*
 * This function serves as the main function of this program
 *
 */
int main() {
    Queue *queue1 = CreateStringQueue(10);
    Queue *queue2 = CreateStringQueue(10);
    Queue *queue3 = CreateStringQueue(10);
    Queue **munch1Queue, **munch2Queue;

    if ((munch1Queue = (Queue**)malloc(sizeof(Queue*) * 2)) == NULL) {
        printf("Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    if ((munch2Queue = (Queue**)malloc(sizeof(Queue*) * 2)) == NULL) {
        printf("Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    
    munch1Queue[0] = queue1;
    munch1Queue[1] = queue2;
    munch2Queue[0] = queue2;
    munch2Queue[1] = queue3;
       
    pthread_t reader, munch1, munch2, writer;
    if (pthread_create(&reader, NULL, &Reader, queue1)) {
        printf("Thread creation failed.\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&munch1, NULL, &Munch1, munch1Queue)) { 
        printf("Thread creation failed.\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&munch2, NULL, &Munch2, munch2Queue)) {
        printf("Thread creation failed.\n");
        exit(EXIT_FAILURE);
    }   
    if (pthread_create(&writer, NULL, &Writer, queue3)) {
        printf("Thread creation failed.\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(reader, NULL) != 0) {
        printf("Thread termination failed.\n");
        exit(EXIT_FAILURE);
    }
    
    if (pthread_join(munch1, NULL) != 0) {
        printf("Thread termination failed.\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(munch2, NULL) != 0) {
        printf("Thread termination failed.\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(writer, NULL) != 0) {
        printf("Thread termination failed.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "PrintQueueStats of the first queue:\n");
    PrintQueueStats(queue1);
    fprintf(stderr, "PrintQueueStats of the second queue:\n"); 
    PrintQueueStats(queue2);
    fprintf(stderr, "PrintQueueStats of the third queue:\n");
    PrintQueueStats(queue3);  
    
    free(queue1->stringQueue);
    free(queue2->stringQueue);
    free(queue3->stringQueue);
    free(queue1);
    free(queue2);
    free(queue3);
    free(munch1Queue);
    free(munch2Queue);
    
    
    return 0;
}

    
