/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#ifndef HOSTD_H_
#define HOSTD_H_

// Include any relevant data structures here thay may be needed
#include "queue.h"
// Include any relevant functions declarations here they must start with the
// extern keyword such as in the following example:
extern void print_process(process proc);

extern void run_process(node_t * pullQueue, node_t * pushQueue, char *argv[]);

#endif /* HOSTD_H_ */
