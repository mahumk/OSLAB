/*
 * Banker's Algorithm for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include "banker.h"

// Put any other macros or constants here using #define
// May be any values >= 0
#define NUM_CUSTOMERS 2
#define NUM_RESOURCES 2


// Put global environment variables here
pthread_t threads[NUM_CUSTOMERS];
pthread_mutex_t lock;
pthread_attr_t attr;
int num;
// Available amount of each resource
int available[NUM_RESOURCES];

// Maximum demand of each customer
int maximum[NUM_CUSTOMERS][NUM_RESOURCES];

// Amount currently allocated to each customer
int allocation[NUM_CUSTOMERS][NUM_RESOURCES];

// Remaining need of each customer
int need[NUM_CUSTOMERS][NUM_RESOURCES];



void *request_res(void *arg);

// Release resources, returns true if successful
//bool release_res(int n_customer, int release[]){

//}


int main(int argc, char *argv[]){
    // ==================== YOUR CODE HERE ==================== //
    srand(time(NULL));

    // Read in arguments from CLI, NUM_RESOURCES is the number of arguments
    if(argc-1==NUM_RESOURCES){
        for(int i=0; i<argc-1; i++){
            available[i] = atoi(argv[i+1]);
        }
    }else{
        printf("%s\n", "Insufficient number of arguments: 3 arguments required");
        EXIT_FAILURE;
    }


    // Allocate the available resources
    for(int i = 0; i < NUM_CUSTOMERS; i++){
        for(int j=0; j< NUM_RESOURCES; j++){
            maximum[i][j] = rand()%(10)+1;
            allocation[i][j] = rand()%(5)+1
        }
    }

    // Initialize the attributes, locks, mutexes, etc.
    num = pthread_attr_init(&attr);
    if(num != 0){
        printf("%s\n", "Failure to Initialize thread attribute");
        EXIT_FAILURE;
    }


    // Create/Run the threads and continually loop
    while(true){
        for(int i=0; i < NUM_CUSTOMERS; i++){
            num= pthread_create(&threads[i], NULL, request_res, NULL);
            if(num != 0){
                printf("%s\n", "Failure to create thread");
                EXIT_FAILURE;
            }
        }
    }




    // The threads will request and then release random numbers of resources

    // If your program hangs you may have a deadlock, otherwise you *may* have
    // implemented the banker's algorithm correctly

    // If you are having issues try and limit the number of threads (NUM_CUSTOMERS)
    // to just 2 and focus on getting the multithreading working for just two threads

    return EXIT_SUCCESS;
}
// Define functions declared in banker.h here
void *request_res(void *arg){

}
