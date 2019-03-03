/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

// Define your utility functions here, you will likely need to add more...

int alloc_mem(resources res, int size){

}

void free_mem(resources res, int index, int size){
    for(int i = index; index < size; i++){
        res.printers = 0;
        res.
    }
}

/*open the file and load each process into a proc struct and add
it to the general queue*/
void load_dispatch(char *dispatch_file, node_t *queue, process tempProc){
    //open the file
    FILE *fp = fopen(dispatch_file,"r");
    char buff[256];
    while(fgets(buff, 256, (FILE)*fp) != NULL){
        char * token = strtok(buff, ",\n");
        tempProc.arrivalTime = atoi(token);
        token = strtok(NULL, ",\n");
        tempProc.priority = atoi(token);
        token = strtok(NULL, ",\n");
        tempProc.processTime = atoi(token);
        token = strtok(NULL, ",\n");
        tempProc.memBytes= atoi(token);
        token = strtok(NULL, ",\n");
        tempProc.resource.printers = atoi(token);
        token = strtok(NULL, ",\n");
        tempProc.resource.scanners = atoi(token);
        token = strtok(NULL, ",\n");
        tempProc.resource.modems = atoi(token);
        token = strtok(NULL, ",\n");
        tempProc.resource.cds = atoi(token);
        tempProc.pid = 0;
        tempProc.memAddress = 0;

        push(queue, tempProc);
    }
    fclose(fp);
}
