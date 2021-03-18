//////////////////////////////////////////////////////////
//Main file: 537make.c
//This file: TextParser.c
//Semester: CS 537 Fall 2020
//
//Author: Xunwei Ye, Chenjie Zhu
//NetID: xye53, czhu226
//CompSci Login: xunwei, chenjie
//Email: xye53@wisc.edu, czhu226@wisc.edu
//////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include "TextParser.h"

const int bufferSize = 4096;

/*
 * This function reads a comment line from the makefile
 *
 * @param lineNum, the number of current line
 * @param makefile, the pointer to the makefile
 * @param text, the pointer to the whole text of makefile
 */
void readCommentLine(int lineNum, FILE *makefile, char** text) {
    char* line;
    if ((line = malloc(sizeof(char) * bufferSize)) == NULL){
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int len = 0;
    char c;
    int nullchar = 0;
    //read the comment line until the end of the line or an error
    while ((c = fgetc(makefile)) != EOF && len < bufferSize - 1){
        if (c == '\n'){
            line[len] = '\0';
            text[lineNum - 1] = line;
            //null character detected, report error
            if (nullchar == 1) {
                line[len] = '\0';
                printf("%i: Null character detected: \"%s\"\n", lineNum, line);
                exit(EXIT_FAILURE);
            }
            return;
        }
        if (c == '\0'){
            nullchar = 1;
            c = '-';
        }
        line[len++] = c;
    }
    //null character detected, report error
    if (nullchar == 1) {
        line[len] = '\0';
        printf("%i: Null character detected: \"%s\"\n", lineNum, line);
        exit(EXIT_FAILURE);
    }
    if (c == EOF){
        line[len] = '\0';
        text[lineNum - 1] = line;
        return;
    }
    else{
        //this line is too long, report error
        line[len] = '\0';
        do{
            c = fgetc(makefile);
        } while (c != EOF && c != '\n');
        printf("%i: This line is too long: \"%s\"\n", lineNum, line);
        exit(EXIT_FAILURE);
    }
    return;
}

/*
 * This function reads a rule line.
 *
 * @param makefile, the pointer to the makefile
 * @param files, the pointer to the array of file
 * @param fileNum, the pointer to the number of files
 * @param lineNum, the number of current line in the makefile
 * @param exist, the number of current target in the array of file
 * @param text, the pointer to the whole text of makefile
 *
 * @return file* the pointer to the array of file
 */
file* readTargetLine(FILE* makefile, file* files, int* fileNum, int lineNum, int* exist, char** text) {
    char* target;
    char* line;
    char c;
    if((target = malloc(sizeof(char) * bufferSize)) == NULL || (line = calloc(bufferSize, sizeof(char))) == NULL){
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    if ((fgets(line, bufferSize, makefile)) == NULL){
        printf("Failed to read file line %i\n", lineNum);
        exit(EXIT_FAILURE);
    }
    int len = strlen(line);
    //this line is too long, report error
    if (len == bufferSize - 1){
        printf("%i: This line is too long: \"%s\"\n", lineNum, line);
        exit(EXIT_FAILURE);
    }
    //this line contains a null character, report error
    if (line[len - 1] != '\n' && !feof(makefile)){
        printf("%i: This line contains a null pointer: \"%s\"\n", lineNum, line);
        exit(EXIT_FAILURE);
    }
    line[len - 1] = '\0';
    text[lineNum - 1] = line;
    fseek(makefile, len * (-1), SEEK_CUR);
    int temp = 0;
    //read the line until a colon, a space or a tab
    while ((c = fgetc(makefile)) != ':' && temp < len && c != ' ' && c != '\t'){
        //report error if no colon detected
        if (c == '\n' || c == '\0'){
            printf("%i: This target line has no colon: \"%s\"\n", lineNum, line);
            exit(EXIT_FAILURE);
        }
        target[temp++] = c;
    }
    if (temp == len){
        printf("%i: Invalid form of target: \"%s\"\n", lineNum, line);
        exit(EXIT_FAILURE);
    }
    target[temp] = '\0';
    //report error if no colon detected
    if (c != ':') {
        while ((c = fgetc(makefile)) != ':'){
            if (c == '\n' || c == '\0'){
                printf("%i: This target line has no colon: \"%s\"\n", lineNum, line);
                exit(EXIT_FAILURE);
            }
        }
    }
    *exist = -1;
    //initialize the file array if there is no file
    if (*fileNum == 0){
        if ((files = malloc(sizeof(file))) == NULL){
           printf("Memory allocation failed\n");
           exit(EXIT_FAILURE);
        }
        *exist = 0;
        files[*exist].name = target;
        files[*exist].number = *exist;
        files[*exist].dependency = NULL;
        files[*exist].cmdNum = 0;
        files[*exist].depNum = 0;
        files[*exist].lineNum = lineNum;
        files[*exist].isTarget = 1;
        (*fileNum)++;
    } else {
        //look up the array of file to see if the target already exist
        for (int i = 0; i < *fileNum; i++){
            //if the target exists
            if (!strcmp(target, files[i].name)){
                *exist = i;
                if(files[i].isTarget != 0){
                    printf("%i: Multiple rules for the same target: \"%s\"\n", lineNum, line);
                    exit(EXIT_FAILURE);
                }
                files[i].isTarget = 1;
                files[i].lineNum = lineNum;
            }
        }
        //if the target doesn't exist
        if (*exist == -1){
            if ((files = realloc(files, sizeof(file) * (*fileNum + 1))) == NULL){
                printf("Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            *exist = *fileNum;
            files[*exist].name = target;
            files[*exist].number = *exist;
            files[*exist].dependency = NULL;
            files[*exist].cmdNum = 0;
            files[*exist].depNum = 0;
            files[*exist].lineNum = lineNum;
            files[*exist].isTarget = 1;
            (*fileNum)++;
        }
    }
    int end = 0;
    //start reading dependencies
    while (end == 0){
        int cur = 0;
        char* dependency;
        //skip all the blank characters
        while((c = fgetc(makefile)) == ' ' || c == '\t'){
        }
        if(c == '\n')
            break;
        else{
            fseek(makefile, -1, SEEK_CUR);
            if ((dependency = malloc(sizeof(char) * bufferSize)) == NULL){
                printf("Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
        }
        //start reading the name of a dependency
        while(end == 0 && (c = fgetc(makefile)) != ' ' && c != '\t'){
            if (c == '\n'){
                end = 1;
                break;
            }
            dependency[cur++] = c;
        }
        dependency[cur] = '\0';
        int found = -1;
        //look up the array of file to see if the dependency already exist
        for (int i = 0; i < *fileNum; i++){
            //if the dependency exist
            if (!strcmp(dependency, files[i].name)){
                found = i;
            }
        }
        //if the dependency doesn't exist
        if (found == -1) {
            if ((files = realloc(files, sizeof(file) * (*fileNum + 1))) == NULL){
                printf("Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            found = *fileNum;
            files[found].name = dependency;
            files[found].number = found;
            files[found].dependency = NULL;
            files[found].cmdNum = 0;
            files[found].depNum = 0;
            files[found].lineNum = lineNum;
            files[found].isTarget = 0;
            (*fileNum)++;
        }
        if(files[*exist].depNum == 0){
            if((files[*exist].dependency = malloc(sizeof(int))) == NULL){
                printf("Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
        } else {
            if((files[*exist].dependency = realloc(files[*exist].dependency, sizeof(int) * (files[*exist].depNum + 1))) == NULL){
                printf("Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
        }
        //set dependency for the target
        (files[*exist].dependency)[(files[*exist].depNum)] = found;
        (files[*exist].depNum)++;
    }
    return files; 
}

/*
 * This function reads a blank line from the makefile
 *
 * @param makefile, the pointer to the makefile
 * @param lineNum, the number of current line in the makefile
 * @param text, the pointer to the whole text of makefile
 */
void readBlankLine(FILE* makefile, int lineNum, char** text){
    char* line;
    if ((line = malloc(sizeof(char) * bufferSize)) == NULL){
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int len = 0;
    char c;
    int valid = 1;
    //read the line until the end or an error
    while ((c = fgetc(makefile)) != EOF && len < bufferSize - 1){
        //if the line ends with no error
        if (c == '\n'){
            line[len] = '\0';
            text[lineNum - 1] = line;
            if (valid == 1)
                return;
            else
                break;
        }
        //if non-blank character detected
        if (c != ' ' && c != '\t')
            valid = 0;
        line[len++] = c;
    }
    if (c == EOF && valid == 1){
        line[len] = '\0';
        text[lineNum - 1] = line;
        return;
    }
    else{
        //if the line contains an error, report error
        line[len] = '\0';
        do{
            c = fgetc(makefile);
        } while (c != EOF && c != '\n');
        if (len == bufferSize - 1)
            printf("%i: This line is too long: \"%s\"\n", lineNum, line);
        else {
            printf("%i: This blank line contains non-blank characters: \"%s\"\n", lineNum, line);
        }
        exit(EXIT_FAILURE);
    }
    return;
}

/*
 * This function reads a command line of the makefile
 *
 * @param makefile, the pointer to the makefile
 * @param lineNum, the number of current line in makefile
 * @param curTarget, the target of this command line
 * @param text, the whole text of the makefile
 * @param the pointer to the array of file
 */
void readCommandLine(FILE *makefile, int lineNum, int* curTarget, char** text, file* files){
    char* line;
    char* cmdline;
    if((line = calloc(bufferSize, sizeof(char))) == NULL || (cmdline = malloc(sizeof(char) * bufferSize)) == NULL){
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    if ((fgets(line, bufferSize, makefile)) == NULL){
        printf("Failed to read file line %i\n", lineNum);
        exit(EXIT_FAILURE);
    }
    int len = strlen(line);
    //if the line is too long, report error
    if (len == bufferSize - 1){
        printf("%i: This line is too long: \"%s\"\n", lineNum, line);
        exit(EXIT_FAILURE);
    }
    line[len] = '-';
    // if the line contains a null character, report error
    if ((int)strlen(line) > len + 1){
        line[strlen(line) - 1] = '\0';
        printf("%i: This line contains a null character: \"%s\"\n", lineNum, line);
        exit(EXIT_FAILURE);
    }
    line[len] = '\0';
    if (line[len - 1] == '\n')
        line[len - 1] = '\0';
    text[lineNum - 1] = line;
    strcpy(cmdline, line);
    if (*curTarget < 0) {
        printf("%i: Command line before any target line: \"%s\"\n", lineNum, line);
        exit(EXIT_FAILURE);
    }
    if (files[*curTarget].cmdNum == 0){
        if((files[*curTarget].cmd = malloc(sizeof(char**))) == NULL || (files[*curTarget].cmdptrs = malloc(sizeof(char*))) == NULL){
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    } else {
        if((files[*curTarget].cmd = realloc(files[*curTarget].cmd, sizeof(char**) * (files[*curTarget].cmdNum + 1))) == NULL || (files[*curTarget].cmdptrs = realloc(files[*curTarget].cmdptrs, sizeof(char*) * (files[*curTarget].cmdNum + 1))) == NULL){
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
    int cmdNum = files[*curTarget].cmdNum;
    if ((files[*curTarget].cmd[cmdNum] = malloc(sizeof(char*))) == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    files[*curTarget].cmdptrs[cmdNum] = cmdline;
    int end = 0;
    int strIndex = 0;
    int argIndex = 0;
    //start reading the command and devide the comamnd into an array of strings
    while (end == 0){
        char* argument;
        //skip all the blank characters
        while(cmdline[strIndex] == ' ' || cmdline[strIndex] == '\t'){
            strIndex++;
        }
        if(cmdline[strIndex] == '\0')
            break;
        else{
            //expand the command array
            if ((files[*curTarget].cmd[cmdNum] = realloc(files[*curTarget].cmd[cmdNum], sizeof(char*) * (argIndex + 2))) == NULL){
                printf("Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            argument = &cmdline[strIndex];
        }
        //read the command
        while(end == 0 && cmdline[strIndex] != ' ' && cmdline[strIndex] != '\t'){
            if (cmdline[strIndex] == '\0'){
                end = 1;
                break;
            }
            strIndex++;
        }
        cmdline[strIndex++] = '\0';
        files[*curTarget].cmd[cmdNum][argIndex++] = argument;
    }
    files[*curTarget].cmd[cmdNum][argIndex] = NULL;
    (files[*curTarget].cmdNum)++;
    return;
}

/*
 * This function checks if all non-target files exist
 * @param files, the array of file
 * @param text, the whole text of makefile
 * @param fileNum, the number of files
 */
void checkExistence(file* files, char** text, int* fileNum){
    for (int i = 0; i < *fileNum; i++){
        if (files[i].isTarget == 0){
            if (access(files[i].name, 0)){
                printf("%i: \"%s\" is neither a target or an existing file: \"%s\"\n", files[i].lineNum, files[i].name, text[files[i].lineNum - 1]);
                exit(EXIT_FAILURE);
            }
        }
    }
}
                
/*
 * This function scan the makefile the parse it into chunks for future use
 *
 * @param fileName, the name of makefile
 * 
 * @return metadata, a struct containing all useful information
 */
metadata parse(char* fileName){
    FILE *makefile;
    //if the user didn't use -f option
    if (fileName == NULL) {
        makefile = fopen("makefile", "r");
        if (makefile == NULL){
            makefile = fopen("Makefile", "r");
            if (makefile == NULL){
                printf("Makefile not found\n");
                exit(EXIT_FAILURE);
            }
        }
    } else {
        //if the user used -f option
        makefile = fopen(fileName, "r");
        if (makefile == NULL){
            printf("%s not found\n", fileName);
            exit(EXIT_FAILURE);
        }
    }
    char firstChar;
    int lineNum = 0;
    file* files = NULL;
    int *fileNum;
    int *curTarget;
    if ((fileNum = malloc(sizeof(int))) == NULL || ((curTarget = malloc(sizeof(int))) == NULL)){
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    *fileNum = 0;
    *curTarget = -1;
    char** lines;
    //read the first character of a line to determine which type of line this is
    while ((firstChar = getc(makefile)) != EOF){
        if (lineNum == 0){
            if ((lines = malloc(sizeof(char*))) == NULL){
                printf("Memory allocation failed\n");
                exit(EXIT_FAILURE);
            } 
        } else {
            if ((lines = realloc(lines, sizeof(char*) * (lineNum + 1))) == NULL){
                printf("Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
        }       
        //if it's a comment line
        if (firstChar == '#'){
            fseek(makefile, -1, SEEK_CUR);
            lineNum++;
            readCommentLine(lineNum, makefile, lines);
        } else if (firstChar == '\t'){
            int len = 1;
            //read the line to determine if it's a command line or a blank line
            while ((firstChar = getc(makefile)) == ' ' || firstChar == '\t'){
                len++;
                if (len == bufferSize) {
                    fseek(makefile, bufferSize * (-1), SEEK_CUR);
                    lineNum++;
                    readBlankLine(makefile, lineNum, lines);
                }
            }
            if(firstChar == EOF || firstChar == '\n'){
                fseek(makefile, len * (-1) - 1, SEEK_CUR);
                lineNum++;
                readBlankLine(makefile, lineNum, lines);
            } else {
                fseek(makefile, len * (-1) - 1, SEEK_CUR);
                lineNum++;
                readCommandLine(makefile, lineNum, curTarget, lines, files);
            }
        } else if (firstChar == ' ' || firstChar == '\n'){
            fseek(makefile, -1, SEEK_CUR);
            lineNum++;
            readBlankLine(makefile, lineNum, lines);
        } else {
            //if it's a rule line
            fseek(makefile, -1, SEEK_CUR);
            lineNum++;
            files = readTargetLine(makefile, files, fileNum, lineNum, curTarget, lines);
        }
    }

    checkExistence(files, lines, fileNum);
    
    metadata data;
    data.files = files;
    data.fileNum = *fileNum;
    data.text = lines;
    data.lineNum = lineNum;

    //start collecting the modification time of each file
    if ((data.modifyTime = calloc(data.fileNum, sizeof(time_t))) == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < data.fileNum; i ++) {
        FILE *fp;
        // file exists in the directory
        if ((fp = fopen(data.files[i].name, "r")) != NULL) { 
        // file is a actual file  
            struct stat sb;
            int fd;
            if ((fd = open(data.files[i].name, O_RDONLY)) < 0) {
                printf("Open file Error!\n");
                exit(EXIT_FAILURE);
            }   

            if ((fstat(fd, &sb)) != 0) {
                printf("Error on fstat()!\n");
                exit(EXIT_FAILURE);
            }
            data.modifyTime[i] = sb.st_mtime;
            fclose(fp);
        }
    }

    if (fclose(makefile) == EOF){
        printf("Failed to close the file\n");
        exit(EXIT_FAILURE);
    }
    free(curTarget);
    free(fileNum);
    return data;
}

