/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"
#include "queue.h"

void init_mem(resources res){
    //init all elements in array to 0
    for(int i=0; i<MEMORY; i++){
        res.mem_avail[i]=0;
    }
}

// Define your utility functions here, you will likely need to add more...
int alloc_mem(int size, process proc, resources res){
    int reserved, address;
    int available=0;

    //check if real time
    // if(proc.priority>0){
    //     reserved = 64;
    // }else{
    //     reserved = 0;
    // }

    //check how much memory is available
    for(int i = reserved+1; i < MEMORY && available<=size; i++){
        //check for consecutive available space
        if(res.mem_avail[i-1] == 0 && res.mem_avail[i] == 0 ){
            available++;
        }else{
            break;
        }
    }
    // if (available == processsize) {
    //for(int i = reserved; i < MEMORY || available > 0; i++){
    // if (mem_avail[i]==0){
    //    mem_avail[i] = 1;
    //    available--;
    // }
    //}
    //}
    //check if more empty memory than whats needed
    if(size<=available){
        for(int i = reserved; i < MEMORY && available > 0; i++){
            //check if being used
            if(res.mem_avail[i] == 0){
                //iterate through size of process
                if(available>0){
                    res.mem_avail[i]=1;
                    available--;
                    // get/update starting address
                    address = i-size;
                }
            }
        }
    }

    return address;
}

int alloc_resources(resources res, process proc){

}



/*open the file and load each process into a proc struct and add
it to the general queue*/
void load_dispatch(char *dispatch_file, node_t *queue, process tempProc){
    //open the file
    FILE *fp = fopen(dispatch_file,"r");
    char buff[256];
    char newline[2] = "\n";
    char comma[2] = ",";
    char *token;
    while(fgets(buff, 256, fp) != NULL){
        //remove the \n
        token = strtok(buff, newline);

        //separate by comma
        token = strtok(token, comma);
        tempProc.arrivalTime = atoi(token);
        token = strtok(NULL, comma);
        tempProc.priority = atoi(token);
        token = strtok(NULL, comma);
        tempProc.processTime = atoi(token);
        token = strtok(NULL, comma);
        tempProc.memBytes= atoi(token);
        token = strtok(NULL, comma);
        tempProc.printers = atoi(token);
        token = strtok(NULL, comma);
        tempProc.scanners = atoi(token);
        token = strtok(NULL, comma);
        tempProc.modems = atoi(token);
        token = strtok(NULL, comma);
        tempProc.resource.cds = atoi(token);
        tempProc.pid = 0;
        tempProc.memAddress = 0;

        //push into general queue
        push(queue, tempProc);
    }
    fclose(fp);
}
