/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// Define your FIFO queue functions here, these will most likely be the
// push and pop functions that you declared in your header file

void push(node_t *queue, process proc){
    node_t *cur = queue;
    //go to the last item in line
    while(cur->next != NULL){
        cur = cur->next;
    }
    //set the end of the line to the new node
    cur->next = (node_t*)malloc(sizeof(node_t));
    //set the number
    cur->next->proc = proc;
    //set the next item to come into the end of the line to be NULL
    cur->next->next = NULL;    
}

process pop(node_t *queue){
    //check if queue is empty
    if(queue->next == NULL){
        exit(0);
    }
    //grab the first item in line
    process popped = queue->next->proc;
    node_t *temp = queue->next; //cur
    //get rid of the first item
    free(temp);
    //since first item gone, the next item points at the item next to that
    queue->next= queue->next->next;
    return popped;
}
