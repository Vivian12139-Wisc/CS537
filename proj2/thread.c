//////////////////////////////////////////////////////////
//Main file: main.c
//This file: thread.c
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//NetID: xye53, czhu226
//CompSci Login: xunwei, chenjie
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include "thread.h"
#include "queue.h"

const int bufferSize = 4096; // the max size of buffer

/*
 * This function is the thread Reader which reads in each input line from stdins
 *
 * @arg void* arg
 * @return NULL
 */
void *Reader(void *arg) {
    Queue *queue1 = (Queue*)arg;
	char *buffer;
    int len;

    if ((buffer = (char*)malloc(sizeof(char) * bufferSize)) == NULL) {
        printf("Failed to allocate memory for a buffer.\n");
        exit(EXIT_FAILURE);
    }
    
    while ((len = getLine(buffer, bufferSize)) < bufferSize + 1) {
        if (len == -1) {
            fprintf(stderr, "The string is too long!\n");
            if ((buffer = (char*)malloc(sizeof(char) * bufferSize)) == NULL) {
                printf("Failed to allocate memory for a buffer.\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (len == bufferSize) {
	        EnqueueString(queue1, buffer);
            break;
        } else { 
            EnqueueString(queue1, buffer);
            if ((buffer = (char*)malloc(sizeof(char) * bufferSize)) == NULL) {
                printf("Failed to allocate memory for a buffer.\n");
                exit(EXIT_FAILURE);
            }
        }    
    }
    
    if (len == bufferSize + 1) 
        fprintf(stderr, "The string is too long!\n");
    
    EnqueueString(queue1, NULL);
	
    pthread_exit(NULL);
    return NULL;     
}

/*
 * This function is the thread Munch1 which will scan the line and replace each space character 
 * with an asterisk ("*") character. It will then pass the line to thread Munch2 through another
 * queue of character strings.
 *
 * @arg void* arg
 * @return NULL
 */
void *Munch1(void *arg) {
    Queue **munch1Queue = (Queue**) arg;
    Queue *queue1 = munch1Queue[0];
    Queue *queue2 = munch1Queue[1];
    char *m1;
    
    while ((m1 = DequeueString(queue1)) != NULL) {
        char *temp = m1;
        while (*temp) {
            if (*temp == ' ') 
                *temp = '*';
            temp++;
        }
        EnqueueString(queue2, m1);
    }
        
    EnqueueString(queue2, NULL); 

    pthread_exit(NULL);
    return NULL;
}

/*
 * This function is the thread Munch2 which scans the line and converts all lower case letters to 
 * upper case with an asterisk ("*") character. It will then pass the line to thread Writer through 
 * another queue of character strings.
 *
 * @arg void* arg
 * @return NULL
 */
void *Munch2(void *arg) {
    Queue **munch2Queue = (Queue**) arg;
    Queue *queue2 = munch2Queue[0];
    Queue *queue3 = munch2Queue[1];
    char *m2;
 
    while ((m2 = DequeueString(queue2)) != NULL) {
        char *temp = m2;
        while (*temp) {
            if (islower(*temp)) 
                *temp = toupper(*temp);
            temp++;
        }
        EnqueueString(queue3, m2);
    }    

    EnqueueString(queue3, NULL);

    pthread_exit(NULL);
    return NULL;
}

/*
 * This function is the thread Writer which writes the line to standard output. 
 *
 * @arg void* arg
 * @return NULL
 */
void *Writer(void *arg) {
    Queue *queue3 = (Queue*) arg;
    char *ws;
    
    while ((ws = DequeueString(queue3)) != NULL) { 
        fprintf(stdout, "%s", ws);
        free(ws);
    } 

    fprintf(stdout, "\nThere have been %d strings processed to stdout.\n", queue3->dequeueCount - 1);

    pthread_exit(NULL);
    return NULL;
}

/*
 * This function read one line from stdin
 *
 * @arg char* line
 * @arg int size
 * @return the length of this line
 */
int getLine(char* line, int size) {
	int c;
	int len = 0;

	while ((c = getc(stdin)) != EOF && len < size) {
		line[len++] = c;
        
        if (len == size) {
            while ((c = getc(stdin)) != EOF) {
                // an oversized line with \n terminated
                if ('\n' == c) 
                  return -1;  
            }
            // an oversized line with EOF terminated
            line = NULL;
            return size + 1;
        }
		if ('\n' == c)
		    break;
	}
    
    // a line is only EOF
    if (c == EOF && len == 0) { 
        line = NULL;
        return size + 2;
    }
    // a normal line with EOF terminated
    else if (c == EOF) {
	    line[len] = '\0';
        return size;
    // a normal line with \n terminated
    } else {
        line[len] = '\0';
        return len; 
    }
}

