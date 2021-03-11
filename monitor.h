/*
 * Author: Nick House
 * Project: Semaphores and Message Passing
 * Course: CS-4760 Operating Systems, Spring 2011
 * File Name: monitor.h
 */

#ifndef MONITOR_H
#define MONITOR_H

#include <stdbool.h>
#include "shared.h"

enum procons{producer, consumer}; 

int makeRandom(int);                     //Create Random Variable 		
int consume(pid_t, int, int, int);       //Consume Variabe at address
bool produce(pid_t, int, int, int);      //Produce Variabe put at address
void openLogfile();                      //Open Logfil 
void closeLogfile();                     //Close Logfile
void semSignal(int); 	                   //Allow driver to signal producers
extern struct sharedMemory *shmptr; 		 //Monitor Access to Shm
extern char logfile[100];                //Logfile Name
extern FILE * logfilePtr;                //Logfile Pointer
extern int shmid;                        //sh Memory Id
extern int shmidSem;                     //Sh Memory Sem id


#endif
