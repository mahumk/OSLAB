/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#ifndef UTILITY_H_
#define UTILITY_H_
#include "queue.h"

// The amount of available memory
#define MEMORY 1024

//Function to intialize memory to 0
extern void init_mem(resources *res);

// Function to allocate a contiguous chunk of memory in your resources structure
// memory array, always make sure you leave the last 64 values (64 MB) free, should
// return the index where the memory was allocated at
extern int alloc_mem(int size, int reserve, resources *res);

extern int alloc_resources(resources *res, process proc);

// Function to free the allocated contiguous chunk of memory in your resources
// structure memory array, should take the resource struct, start index, and
// size (amount of memory allocated) as arguments
extern void free_mem(resources *res, int index, int size);

extern void free_resources(resources *res, process proc);

// Function to parse the file and initialize each process structure and add
// it to your job dispatch list queue (linked list)
extern void load_dispatch(char *dispatch_file, node_t *queue, process tempProc);


#endif /* UTILITY_H_ */
