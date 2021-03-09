/*
 * Author: Nick House
 * Project: Semaphores and Message Passing
 * Course: CS-4760 Operating Systems, Spring 2011
 * File Name: monitor.h
 */

#ifndef MONITOR_H
#define MONITOR_H

#include "shared.h"

//enum prod_cons{producer, consumer}; 

int makeRandom(int);                     //Create Random Variable 		
int * consume(pid_t, int, int, int);     //Consume Variabe at address
int * produce(pid_t, int, int, int);     //Produce Variabe put at address
void openLogfile(); 
void closeLogfile(); 

extern struct sharedMemory *shmptr; 		 //Monitor Access to Shm
extern char logfile[100];                //Logfile Name
extern FILE * logfilePtr;                //Logfile Pointer
//extern key_t key; 
//extern size_t memSize; 
extern int shmid; 
//extern key_t semKey; 
extern int shmidSem; 


#endif
