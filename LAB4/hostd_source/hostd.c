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
node_t * tmpQueue = NULL;

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
    tmpQueue = malloc(sizeof(node_t));

    //point to start of queue
    jobQueue->next = NULL;
    runtime->next = NULL;
    priority1->next = NULL;
    priority2->next = NULL;
    priority3->next = NULL;
    tmpQueue->next = NULL;

    // Load the dispatchlist
    // Add each process structure instance to the job dispatch list queue
    load_dispatch("dispatchlist.txt", jobQueue, job);


    //initialize the memory
    init_mem(&res);

    // Iterate through each item in the job dispatch list, add each process
    // to the appropriate queues
    while(jobQueue->next != NULL){
        job = pop(jobQueue);
        if(job.priority ==3){
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
    process *tmpjob;

    //Allocate memory for as many runtime processes as possible and run
    while(runtime->next != NULL){
        tmpjob = &runtime->next->proc;

        //store in memory if possible
        if((address = alloc_mem(tmpjob->memBytes, 0, &res)) != -1){
            tmpjob->memAddress = address;
        }else{
            //no available memory so run to free memory
            run_process(runtime, runtime, argv);
        }
        //run if no problems
        run_process(runtime, runtime, argv);
    }

    clear_mem(&res);
    while(priority1->next != NULL){
        tmpjob = &priority1->next->proc;

        address = alloc_mem(tmpjob->memBytes, 64, &res);
        allocated = alloc_resources(&res, *tmpjob);

        //store in memory if possible
        if(address != -1){
            tmpjob->memAddress = address;
            //reserve resources if availab;e
            if(allocated != 0){
                tmpjob->allocated = allocated;

                //push into queue to be run and rmv from this queue
                push(tmpQueue,*tmpjob);
                pop(priority1);
            }else{
                //if out of resources run whatever is in the runnable queue to free resources/memory
                while((allocated = alloc_resources(&res, *tmpjob)) == 0){
                    run_process(tmpQueue, priority2, argv);
                }
                //reserve resources for new process
                tmpjob->allocated = allocated;

                //yeet into queue to be run and rmv from this queue
                push(tmpQueue,*tmpjob);
                pop(priority1);

                //if it's the last item to be run the run
                if(tmpQueue->next->next == NULL && priority1->next == NULL){
                    run_process(tmpQueue, priority2, argv);
                }
            }
        }else{
            run_process(tmpQueue, priority2, argv);
        }
    }

    clear_mem(&res);

    while(priority2->next != NULL){
        tmpjob = &priority2->next->proc;

        address = alloc_mem(tmpjob->memBytes, 64, &res);
        allocated = alloc_resources(&res, *tmpjob);

        //store in memory if possible
        if(address != -1){
            tmpjob->memAddress = address;
            //reserve resources if availab;e
            if(allocated != 0){
                tmpjob->allocated = allocated;
                //push into queue to be run and rmv from this queue
                push(tmpQueue,*tmpjob);
                pop(priority2);
            }else{
                //if out of resources run whatever is in the runnable queue to free resources/memory
                while((allocated = alloc_resources(&res, *tmpjob)) == 0){
                    run_process(tmpQueue, priority3, argv);
                }

                tmpjob->allocated = allocated;

                //yeet into queue to be run and rmv from this queue
                push(tmpQueue,*tmpjob);
                pop(priority2);


            }
            //if it's the last item to be run the run
            if(tmpQueue->next->next == NULL && priority2->next == NULL){
                //printf("RUNNING2222: %d\n", tmpQueue->next->proc.arrivalTime);
                run_process(tmpQueue, priority3, argv);
            }
        }else{
            if(tmpQueue->next->next != NULL){
                run_process(tmpQueue, priority3, argv);
            }else{
                push(priority2, pop(priority2));
                clear_mem(&res);
            }
        }
    }
    clear_mem(&res);
    while(priority3->next != NULL){
        tmpjob = &priority3->next->proc;

        address = alloc_mem(tmpjob->memBytes, 64, &res);
        allocated = alloc_resources(&res, *tmpjob);

        //store in memory if possible
        if(address != -1){
            tmpjob->memAddress = address;
            //reserve resources if availab;e
            if(allocated != 0){
                tmpjob->allocated = allocated;
                //push into queue to be run and rmv from this queue
                push(tmpQueue,*tmpjob);
                pop(priority3);
            }else{
                //if out of resources run whatever is in the runnable queue to free resources/memory
                while((allocated = alloc_resources(&res, *tmpjob)) == 0 && tmpQueue->next->next != NULL){
                    run_process(tmpQueue, priority3, argv);
                }
                //if still not enough resources and nothing to run
                if(allocated == 0){
                    reset_resources(&res);
                    allocated = alloc_resources(&res, *tmpjob);
                }
                tmpjob->allocated = allocated;

                //yeet into queue to be run and rmv from this queue
                push(tmpQueue,*tmpjob);
                pop(priority3);


            }
            //if it's the last item to be run the run
            if(tmpQueue->next->next == NULL && priority3->next == NULL){
                run_process(tmpQueue, priority3, argv);
            }
        }else{
            if(tmpQueue->next->next != NULL){
                run_process(tmpQueue, priority3, argv);
            }else{
                push(priority3, pop(priority3));
                clear_mem(&res);
            }
        }
    }

    return EXIT_SUCCESS;
}

void print_process(process proc){
    printf("Arrival Time: %d Priority: %d Time Left: %d Size: %dbytes Printers: %d Scanners: %d Modems: %d CDs: %d\n", proc.arrivalTime, proc.priority, proc.processTime, proc.memBytes, proc.printers, proc.scanners, proc.modems, proc.cds);
}

// Allocate memory and resources for each process before it's executed
// Execute the process binary using fork and exec
// Perform the appropriate signal handling / resource allocation and de-alloaction
void run_process(node_t * pullQueue, node_t * pushQueue, char *argv[]){
    int stat;
    pid_t pid;

    process *proc;
    process popjob;
    popjob = pop(pullQueue);
    proc = &popjob;
    //create a parent/child process
    pid = fork();
    proc->pid = (int) pid;

    //make sure the process created successfully
    if(pid < (pid_t) 0){
        stat = -1;
        exit(1);
    }

    if(pid == 0){ //child runs sigtrap

        execv("./process", argv);
    }else{ //parent
        print_process(*proc);

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
            if(proc->paused == 0){
                //run for 1s
                sleep(1);
                proc->processTime--;
                //suspend job
                kill(proc->pid, SIGTSTP);
                proc->paused = 1;

            }else{
                //resume if paused
                kill(proc->pid, SIGCONT);
                //run for 1s
                sleep(1);
                proc->processTime--;
                //suspend job
                kill(proc->pid, SIGTSTP);
                proc->paused = 1;

            }
            //free mem & resources after runned for 1s
            free_mem(&res, proc->memAddress, proc->memBytes);
            free_resources(&res, *proc);

            //update run time and priority
            if(proc->priority == 1){
                proc->priority++;
            }else if(proc->priority ==2){
                proc->priority++;
            }else if(proc->priority == 3){
                proc->priority=3;
            }
            //terminate signal if process time done
            if(proc->processTime <= 0){
                sleep(1);
                proc->processTime--;
                kill(proc->pid, SIGINT);

            }else{
                push(pushQueue, *proc);
            }

        }

    }
}
