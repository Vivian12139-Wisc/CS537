//////////////////////////////////////////////////////////
//Main file: 537make.c
//This file: SpecGraph.c
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//NetID: xye53, czhu226
//CompSci Login: xunwei, chenjie
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "SpecGraph.h"
#include "TextParser.h"

/*
 * This function performs a depth first search in the graph
 *
 * @param data, containing the graph
 * @param curTarget, the current node of search
 * @param lastTarget, last node of search
 * @param visited, an array of integers recording if the nodes are visited
 */
void DFS(metadata data, int curTarget, int lastTarget, int* visited){
    if (visited[curTarget] == 1){
        int lineNum = data.files[lastTarget].lineNum;
        printf("%i: A cycle is detected in the chain of dependencies:\"%s\"\n", lineNum, data.text[lineNum - 1] );
        exit(EXIT_FAILURE);
    } else
        visited[curTarget] = 1;
    for (int i = 0; i < data.files[curTarget].depNum; i++)
        DFS(data, data.files[curTarget].dependency[i], curTarget, visited);
    visited[curTarget] = 0;
    return;
}

/*
 * This function sees if there's a cycle in the graph
 * 
 * @param data, containing the graph
 */
void detectCycle(metadata data){
    int* visited;
    if((visited = calloc(data.fileNum, sizeof(int))) == NULL){
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < data.fileNum; i++)
        DFS(data, i, i, visited);
    free(visited);
    return;
}

