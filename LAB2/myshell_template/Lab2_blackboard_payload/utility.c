/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2017, <GROUP MEMBERS>
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

// Define your utility functions here, these will most likely be functions that you call
// in your myshell.c source file

void pauseShell(){
    //while the enter key has not been pressed, sleep
    while(getchar() != '\n'){
        printf("%s\n","Paused ...");
        sleep(10);
    }
}
void helpManual(){
    //I didnt do the more command but this is just the help command
    printf("Commands Available:   \n");
    printf("echo: prints to screen\n");
    printf("pause: pauses command shell\n");
    printf("quit: exits shell\n");
    printf("cd: changes directory\n");
    printf("clr: clears screen\n");
    printf("dir: displays contents of directory\n");
    printf("environ: displays all environments\n\n");
}

void listEnv(){
    //list all the environemnts on the user PATH (didnt do home or root)
    printf("PATH : %s\n", getenv("PATH"));
}

void clear(){
    //print 50 blank lines to clear the screen
    for(int i=0; i< 50; i++){
        printf("\n");
    }
}
