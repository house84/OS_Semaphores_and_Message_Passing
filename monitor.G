/* 
 * Author: Nick House
 * Project: Semaphores and Message Passing
 * Course: CS-4760 Operating Systems, Spring 2021
 * File Name: monitor.h
*/

#ifndef MONITOR_H
#define MONITOR_H

#include "shared.h" 

int makeRandom(int); 								//Create Random Variable 
int addProduct(int *); 								//Add Product to Buffer
int * produce(pid_t, int);						//produce Func
int * consume(pid_t, int);						//consume Func
int * consume(); 											//Consume Product return address
void openLogFile(); 									//Set Log File PTR Create File
void closeLogFile(); 									//Close Log File
void semWait(int); 										//Wait/Decrement Sem Count
void semSignal(int); 									//Wait/Incrment Sem Count
extern FILE * logFilePtr;							//Set Logfile Ptr
extern char logfile[100]; 						//Logfile Name
extern struct sembuf sops; 						//struct for semops
extern struct sharedMemory *shmptr; 	//Shared Memory Pointer 

#endif
