/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#ifndef QUEUE_H_
#define QUEUE_H_

#define MEMORY 1024
// Resources structure containing integers for each resource constraint and an
// array of 1024 for the memory
typedef struct {
    int printers;
    int scanners;
    int modems;
    int cds;
    int *mem_avail[MEMORY];
} resources;


// Processes structure containing all of the process details parsed from the
// input file, should also include the memory address (an index) which indicates
// where in the resources memory array its memory was allocated
typedef struct {
    int arrivalTime;
    int priority;
    int processTime;
    int memBytes;
    int printers;
    int scanners;
    int modems;
    int cds;
    int pid;
    int memAddress;
    int allocated;
    int suspended;
} process;


// Your linked list structure for your queue
typedef struct node{
    process proc;
    struct node *next;
} node_t;



// Add a new process to the queue, returns the address of the next node added
// to the linked list
extern void push(node_t *queue, process proc);

//removes the element in the last node
extern process pop(node_t *queue);


#endif /* QUEUE_H_ */
