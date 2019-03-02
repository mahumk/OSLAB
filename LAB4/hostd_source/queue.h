/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#ifndef QUEUE_H_
#define QUEUE_H_

#include "utility.h"

// Your linked list structure for your queue
typedef struct node{
    proc process;
    struct node *next;
} node_t;


// Add a new process to the queue, returns the address of the next node added
// to the linked list
extern node_t *push(node_t *tail, proc process);

//removes the element in the last node
extern node_t *pop(node_t *tail);


#endif /* QUEUE_H_ */
