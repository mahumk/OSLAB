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

int main(int argc, char *argv[]){
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
        }else if(job.priority >1){
            push(priority2, job);
        }else if(job.priority >0){
            push(priority1, job);
        }else{
            push(runtime, job);
        }
    }

    int address, allocated;

    //Allocate memory for as many runtime processes as possible and run
    while(runtime->next != NULL){
        job = runtime->next->proc;

        //store in memory if possible
        if((address = alloc_mem(job.memBytes, 0, &res)) != -1){
            job.memAddress = address;
        }else{
            //no available memory so run to free memory
            run_process(runtime, runtime, argv);
        }
        //run if no problems
        run_process(runtime, runtime, argv);

        printf("%d Address: %d\n",job.arrivalTime, job.memAddress);
    }
    int i=1;
    while(priority1->next != NULL){
        job = priority1->next->proc;

        address = alloc_mem(job.memBytes, 64, &res);
        allocated = alloc_resources(&res, job);
        //store in memory and reserve resources if possible
        if(address != -1 && allocated != 0){
            job.memAddress = address;
            job.allocated = allocated;
        }else{
            //no available memory so run to free memory
            // run_process(priority1, priority2, argv);
            // printf("Num items Q1: %d\n", i);
        }
        run_process(priority1, priority2, argv);
        printf("Num items Q1: %d\n", i);
        i++;
    }

    // Repeat until all processes have been executed, all queues are empty
    i=1;
    while(priority1->next != NULL){
        run_process(priority1, priority2, argv);
        printf("Num items LEFTOVER Q1: %d\n", i);
        i++;
    }

    i=1;
    while(priority2->next != NULL){
        job = priority2->next->proc;

        address = alloc_mem(job.memBytes, 64, &res);
        allocated = alloc_resources(&res, job);
        //store in memory and reserve resources if possible
        if(address != -1 && allocated != 0){
            job.memAddress = address;
            job.allocated = allocated;
        }else{
            //no available memory so run to free memory
            // run_process(priority2, priority3, argv);
            // printf("Num items Q2: %d\n", i);
        }
        run_process(priority2, priority3, argv);
        printf("Num items Q2: %d\n", i);
        i++;
    }

    // Repeat until all processes have been executed, all queues are empty
    i=1;
    while(priority2->next != NULL){
        run_process(priority2, priority3, argv);
        printf("Num items LEFTOVER Q2: %d\n", i);
        i++;
    }

    i=1;
    while(priority3->next != NULL){
        job = priority3->next->proc;

        address = alloc_mem(job.memBytes, 64, &res);
        allocated = alloc_resources(&res, job);
        //store in memory and reserve resources if possible
        if(address != -1 && allocated != 0){
            job.memAddress = address;
            job.allocated = allocated;
        }else{
            //no available memory so run to free memory
            // run_process(priority3, priority3, argv);
            // printf("Num items Q3: %d\n", i);
        }
        run_process(priority3, priority3, argv);
        printf("Num items Q3: %d\n", i);
        i++;
    }

    // Repeat until all processes have been executed, all queues are empty
    i=1;
    while(priority3->next != NULL){
        run_process(priority3, priority3, argv);
        printf("Num items LEFTOVER Q3: %d\n", i);
        i++;
    }

    printf("%s\n", "The End");
    return EXIT_SUCCESS;
}

// Allocate memory and resources for each process before it's executed
// Execute the process binary using fork and exec
// Perform the appropriate signal handling / resource allocation and de-alloaction
void run_process(node_t * pullQueue, node_t * pushQueue, char *argv[]){
    int stat;
    process *proc;
    process popProc;
    //get the job to be run
    popProc = pop(pullQueue);
    proc = &popProc;

    pid_t pid;

    pid = fork();
    proc->pid = (int) pid;

    if(pid < (pid_t) 0){
        stat = -1;
        exit(1);
    }

    if(pid == 0){ //child runs sigtrap
        execv("./process", argv);
    }else{ //parent
        //if priority is runtime then run until the job is done
        if(proc->priority == 0){
            //stop the process
            sleep(proc->processTime);
            kill(proc->pid, SIGINT);
            waitpid(proc->pid, &stat, 0);

            //free from memory
            free_mem(&res, proc->memAddress, proc->memBytes);

        }else{
            //check if not paused
            if(proc->suspended == 0){
                //run for 1s
                sleep(1);
                proc->processTime--;
                printf("PID: %d Time: %d\n",proc->pid, proc->processTime);
                //suspend job
                kill(proc->pid, SIGTSTP);
                proc->suspended = 1;

                //update run time and priority
                if(proc->priority == 1 || proc->priority ==2){
                    proc->priority++;
                    //reset
                    proc->pid = 0;
                    proc->memAddress = -1;
                    proc->allocated = 0;
                }else if(proc->priority == 3){
                    proc->priority = 3;
                    proc->pid = 0;
                    proc->memAddress = -1;
                    proc->allocated = 0;
                }

                //push to lower queue
                push(pushQueue, *proc);
                printf("%s\n","Push to next priority");

                if(proc->processTime <=1){
                    //terminate signal if process time done
                    kill(proc->pid, SIGINT);
                    waitpid(proc->pid, &stat, 0);
                    pop(pushQueue);

                    //free memory and resources when done
                    free_mem(&res, proc->memAddress, proc->memBytes);
                    free_resources(&res, *proc);
                    printf("%s\n","free");
                }

            }else{
                //resume if suspended
                kill(proc->pid, SIGCONT);
                proc->suspended = 0;

                //run for 1s
                sleep(1);
                proc->processTime--;
                printf("PID: %d Time: %d\n",proc->pid, proc->processTime);

                //suspend job
                kill(proc->pid, SIGTSTP);

                //update run time and priority
                if(proc->priority == 1 || proc->priority ==2){
                    proc->priority++;
                    //reset
                    proc->pid = 0;
                    proc->memAddress = -1;
                    proc->allocated = 0;
                }else if(proc->priority == 3){
                    proc->priority = 3;
                    proc->pid = 0;
                    proc->memAddress = -1;
                    proc->allocated = 0;
                }

                //push to lower queue
                push(pushQueue, *proc);
                printf("%s\n","CONTINUES Push to next priority");

                if(proc->processTime <=1){
                    //terminate signal if process time done
                    kill(proc->pid, SIGINT);
                    waitpid(proc->pid, &stat, 0);
                    pop(pushQueue);

                    //free memory and resources when done
                    free_mem(&res, proc->memAddress, proc->memBytes);
                    free_resources(&res, *proc);
                    printf("%s\n","free");
                }
            }

        }

    }
}
