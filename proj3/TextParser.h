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

#ifndef TEXTPARSER_H_INCLUDED
#define TEXTPARSER_H_INCLUDED

// the information of a dependency or a target
typedef struct{
    int number; // the number of the file
    char* name; // the name of the file
    int* dependency; // the numbers of dependencies of the file
    char*** cmd; // the chunks of commands of the file 
    char** cmdptrs; // the whole command lines
    int cmdNum; // the number of commands
    int depNum; // the number of dependencies
    int lineNum; // the number of line that introduced this file
    int isTarget;// =1 if it's a target, =0 if it's not
} file;

typedef struct{
    file* files; // the array of files
    int fileNum; // the number of files
    char** text; // the whole text of makefile
    int lineNum; // the number of lines in the makefile
    time_t* modifyTime; //the modification times of files
} metadata;

void readCommentLine(int lineNum, FILE *makefile, char** text);
file* readTargetLine(FILE* makefile, file* files, int* fileNum, int lineNum, int* exist, char** text);
void readBlankLine(FILE* makefile, int lineNum, char** text);
void readCommandLine(FILE *makefile, int lineNum, int* curTarget, char** text, file* files);
void checkExistence(file* files, char** text, int* fileNum);
metadata parse(char* fileName);

#endif

