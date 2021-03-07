/*
 * Author: Nick House
 * Project: Semaphores and Messge Passing
 * Course: CS-4760 Operating Systems, Spring 2021
 * File Name: shared.h
 */

#ifndef SHARED_H
#define SHARED_H 

#include <semaphore.h>

struct sharedMemory {
  
//	extern semaphore mutex; 				//To Get Exclusive access to Buffer
//	extern semaphore empty(20); 		//Number of available Buffers
//	extern semaphore full(0); 			//Initialize to Zero
	
	
	//Dev Testing Variables
	int x; 
	char  logFileName[100]; 

};


#endif
