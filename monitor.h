/*
 * Author: Nick House
 * Project: Semaphores and Message Passing
 * Course: CS-4760 Operating Systems, Spring 2011
 * File Name: monitor.h
 */

#ifndef MONITOR_H
#define MONITOR_H

#include "shared.h"

enum prod_cons{producer, consumer}; 

int makeRandom(int);                     //Create Random Variable 		
int * consume(pid_t, int);               //Consume Variabe at address
int * produce(pid_t, int);               //Produce Variabe put at address

extern struct sharedMemory *shmptr; 		 //Monitor Access to Shm
//extern logfile[100];                     //Logfile Name
//extern FILE * logfilePtr;                //Logfile Pointer

#endif
