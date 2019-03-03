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
proc job;
resources res;

node_t * jobQueue;
node_t * runtime;
node_t * priority1;
node_t * priority2;
node_t * priority3;
// Define functions declared in hostd.h here

int main(int argc, char *argv[]){
    // ==================== YOUR CODE HERE ==================== //
    char dispatchlist = "dispatchlist.txt";

    //assigning Resources
    res = {PRINTERS, SCANNERS, MODEMS, CDS};

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
    load_dispatch(dispatchlist, jobQueue, job);

    // Iterate through each item in the job dispatch list, add each process
    // to the appropriate queues
    while(jobQueue->next != NULL){
        job = pop(jobQueue);

        if(job.priority >2){
            push(priority3, job);
        }else if(job.priority >1){
            push(priority2, job);
        }else if(job.priority >0){
            push(priority1, job);
        }else{
            push(runtime, job);
        }

        //iterate to next job
        jobQueue = jobQueue->next;
    }    

    // Allocate the resources for each process before it's executed

    // Execute the process binary using fork and exec

    // Perform the appropriate signal handling / resource allocation and de-alloaction

    // Repeat until all processes have been executed, all queues are empty

    return EXIT_SUCCESS;
}
