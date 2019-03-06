/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "queue.h"
#include "utility.h"
#include "hostd.h"

// Put macros or constants here using #define
#define MEMORY 1024
#define PRINTERS 2
#define SCANNERS 1
#define MODEMS 1
#define CDS 2

// Put global environment variables here
process job;
resources res;

node_t *jobQueue = NULL;
node_t * runtime = NULL;
node_t * priority1 = NULL;
node_t * priority2 = NULL;
node_t * priority3 = NULL;

// Define functions declared in hostd.h here
void print_process(process proc);

int main(){
    // ==================== YOUR CODE HERE ==================== //
    //assigning MAX Resources
    res.printers= PRINTERS;
    res.scanners = SCANNERS;
    res.modems = MODEMS;
    res.cds = CDS;

    //reserving space for queues
    jobQueue = malloc(sizeof(node_t));
    runtime = malloc(sizeof(node_t));
    priority1 = malloc(sizeof(node_t));
    priority2 = malloc(sizeof(node_t));
    priority3 = malloc(sizeof(node_t));

    //point to start of queue
    jobQueue->next = NULL;
    runtime->next = NULL;
    priority1->next = NULL;
    priority2->next = NULL;
    priority3->next = NULL;

    // Load the dispatchlist
    // Add each process structure instance to the job dispatch list queue
    load_dispatch("dispatchlist.txt", jobQueue, job);


    //initialize the memory
    init_mem(&res);

    // Iterate through each item in the job dispatch list, add each process
    // to the appropriate queues
    while(jobQueue->next != NULL){
        job = pop(jobQueue);
        if(job.priority >2){
            push(priority3, job);
            printf("%s\n", "3");
        }else if(job.priority >1){
            push(priority2, job);
            printf("%s\n", "2");
        }else if(job.priority >0){
            push(priority1, job);
            printf("%s\n", "1");
        }else{
            push(runtime, job);
            printf("%s\n", "0");
        }
    }

    //Allocate memory for as many processes
    while(runtime->next != NULL){
        job = runtime->next->proc;
        int address;
        //store in memory if possible
        if((address = alloc_mem(job.memBytes, 0, &res)) != -1){
            job.memAddress = address;
        }
        printf("%d %d\n", job.arrivalTime, job.memAddress);
        runtime = runtime->next;
    }

    // Allocate the resources for each process before it's executed

    // Execute the process binary using fork and exec

    // Perform the appropriate signal handling / resource allocation and de-alloaction

    // Repeat until all processes have been executed, all queues are empty

    return EXIT_SUCCESS;
}
