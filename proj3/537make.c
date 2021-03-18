//////////////////////////////////////////////////////////
//Main file: 537make.c
//This file: 537make.c
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//NetID: xye53, czhu226
//CompSci Login: xunwei, chenjie
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TextParser.h"
#include "SpecGraph.h"
#include "SpecRepresent.h"

/*
 * This function is the main function of the whole program
 *
 * @param argc, the number of arguments
 * @param argv, array of arguments
 */
int main(int argc, char* argv[]) {
    if (argc < 0 || argc > 4 || argv == NULL) {
        printf("Wrong command line arguments. Usage: 537make [-f fileName] [target]\n");
        exit(EXIT_FAILURE);
    }
    
    int target = 0;
    char* fileName = NULL;
    
    if (argc > 2) {
        if (!strcmp(argv[1], "-f")) {
            fileName = argv[2];
            if (argc == 4)
                target = 3;
        } else if (!strcmp(argv[2], "-f") && argc == 4) {
            fileName = argv[3];
            target = 1;
        } else {
            printf("Wrong command line arguments. Usage: 537make [-f fileName] [target]\n");
            exit(EXIT_FAILURE);
        }
    } else if (argc == 2)
        target = 1;
    
    metadata data;
    data = parse(fileName);
    detectCycle(data);
        
    if (target == 0) {
        runRules(data.files[0].name, data);
    } else {
        runRules(argv[target], data);
    }
        
    for (int i = 0; i < data.fileNum; i++) {
        for (int j = 0; j < data.files[i].cmdNum; j++){
            free(data.files[i].cmdptrs[j]);
            free(data.files[i].cmd[j]);
        }
        free(data.files[i].dependency);
        free(data.files[i].name);
        free(data.files[i].cmd);
    }
    free(data.files);
    free(data.modifyTime);
    for (int i = 0; i < data.lineNum; i++)
        free(data.text[i]);

    return 0;
}

