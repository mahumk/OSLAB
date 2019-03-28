/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2017, <Cassidy, Mahum, Shivani>
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "utility.h"
#include "myshell.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256
// Put global environment variables here

//Define functions declared in myshell.h here
FILE * stream(FILE * f){
    if(f != NULL){
        return f;
    }else{
        return stdin;
    }
}
int main(int argc, char *argv[]){
    // Input buffer and commands
    char buffer[BUFFER_LEN] = {0};
    char command[BUFFER_LEN] = {0};
    char args[BUFFER_LEN][BUFFER_LEN] = {{0}};
    char cwd[1024];
    FILE *fp;

    //hecks if shell script has been provided
    if(argc >= 2){
        if(strstr(argv[1], ".sh") != NULL){
            fp = fopen(argv[1], "r");
        }
    }


    printf("%s$ ", getcwd(cwd, sizeof(cwd)));



    // Perform an infinite loop getting command input from users
    while (fgets(buffer, BUFFER_LEN, stream(fp)) != NULL){
    // Perform string tokenization to get the command and argument
        //get the command
        char* token = strtok(buffer, " ");
        strcpy(command, token);

        //get args
        int numWords = 0;
        while(token != NULL){
            token = strtok(NULL, " ");
            if(token != NULL){
                strcpy(args[numWords], token);
                numWords++;
            }
        }

        //remove \n from args array
        for(int i=0; i<numWords; i++){
            for(int j=0; j <strlen(args[i]); j++){
                if(args[i][j] == '\n' || args[i][j] == '\0'){
                    args[i][j] = NULL;
                }
            }
        }


        // Check the command and execute the operations for each command
        if (strcmp(command, "cd") == 0 || strcmp(command, "cd\n") == 0){
            // cd command -- change the current directory
            //changeDir(args, cwd);
            //if nothing is entered then the current directory is stated
            if(strlen(args[0]) <=0){
                printf("PWD: %s\n", getcwd(cwd, sizeof(cwd)));
            }else{
                //checks if the directory exist otherwise change directory
                if(chdir(args[0])< 0){
                    fputs("Cannot find directory\n", stderr);
                }else{
                    chdir(args[0]);
                    printf("Directory: %s\n", getcwd(cwd, sizeof(cwd)));
                }
            }
        }
        else if(strcmp(command, "dir") == 0 || strcmp(command, "dir\n") == 0){
            //listDir(args);
            DIR *d;
            struct dirent *dir;
            //opens directory
            d = opendir(args[0]);
            //check if directory opened
            if(d){
                //read items in the directory and print em
                while((dir = readdir(d)) != NULL){
                    printf("args%s\n", dir -> d_name);
                }
                closedir(d);
            }
        }
        else if(strcmp(command, "echo") == 0 || strcmp(command, "echo\n") == 0){
            for(int i=0; i <=numWords; i++){
                printf("%s ", args[i]);
            }
            printf("\n");
            // for(int i=0; i <=numWords; i++){
            //     args[i]="";
            // }
        }

        else if (strcmp(command, "pause") == 0 || strcmp(command, "pause\n") == 0){
            pauseShell();
        }
        else if(strcmp(command, "help") == 0 || strcmp(command, "help\n") == 0){
            helpManual();
        }
        else if(strcmp(command, "environ") == 0 || strcmp(command, "environ\n") == 0){
            listEnv();
        }

        else if(strcmp(command, "clr") == 0 || strcmp(command, "clr\n") == 0){
            clear();
        }

        else if (strcmp(command, "quit") == 0 || strcmp(command, "quit\n") == 0){
            // quit command -- exit the shell
            return EXIT_SUCCESS;
        }
        else{
            // Unsupported command
            fputs("Unsupported command, use help to display the manual\n", stderr);
        }

        //clear the arrays for new input
        memset(command, 0, BUFFER_LEN);
        memset(args, 0, sizeof(args[BUFFER_LEN][BUFFER_LEN]));

        //update current directory
        printf("%s$ ", getcwd(cwd, sizeof(cwd)));
    }
    return EXIT_SUCCESS;
}
