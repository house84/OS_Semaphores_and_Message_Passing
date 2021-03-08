/*
 * Author: Nick House
 * Project: Semaphores and Messge Passing
 * Course: CS-4760 Operating Systems, Spring 2021
 * File Name: shared.h
 */

#ifndef SHARED_H
#define SHARED_H 

#include <semaphore.h>
#define semBufLength 10									//set buffer Length for global Access

enum Sems{mutex, availableSpace, availableProducts}; 
//mutex = 1   													Ensure Mutual Exclusion
//availableSpace = semBufLenth					Size of Buffer: Amount of space available to producer
//availableProducts = 0 								Amount of Products available to Consumer 		

struct sharedMemory {
	
	//Dev Testing Variables
	int x; 																//Testing Int
	char  logFileName[100];								//Logfile Name
	int * semBuffer[semBufLength]; 				//memory Addresses for Products Initialize to NULL

};


#endif
