//////////////////////////////////////////////////////////
//Main file: 537make.c
//This file: TextParser.h
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//NetID: xye53, czhu226
//CompSci Login: xunwei, chenjie
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////

#ifndef SPECGRAPH_H_INCLUDED
#define SPECGRAPH_H_INCLUDED
#include "TextParser.h"

void DFS(metadata data, int curTarget, int lastTarget, int* visited);
void detectCycle(metadata data);

#endif
