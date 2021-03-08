/*
 * Author: Nick House
 * Project: Semaphores and Message Passing
 * Course: CS-4760 Operating Systems, Spring 2021
 * File Name: producer.c
 */

#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "shared.h"

//semWait() Decrements and waits 
//semSignal() Increments

//struct sharedMemory *shmptr; 				//Get Access to shared memory

int * produce(); 										//Produce pointer of random variable
void addProduct(int *p); 						//Add Product to Buffer in shared Memory
struct sharedMemory *shmptr; 				//Shared Mem Pointer

//Pass as args shmid, shmidSem, 
int main(int argc, char *argv[]){

	//Handle Args and create SHM and SEM

	int * x = produce(); 						//Return pointer to random Int		
	
	semWait(availableSpace); 				//wait for buffer Space

	semWait(mutex); 								//wait for mutual exclusion
	
	addProduct(x);									//function to add product to non null space in shm array
	
	semSignal(mutex);								//Give Up mutual exclusion
	
	semSignal(availableProducts); 	//Increment available products Semaphore. 
	

	return 0; 

}


//Produce Random Variable
int * produce(){

	srand(time(NULL)); 
	int r = rand()/1000; 
	printf("Rand: %d\n", r); 
	int *rptr; 
	rptr = &r; 

	return rptr; 
}


//Add Product to buffer array in SHM
void addProduct(int * p){

	//Test Call
	printf("TEST add Product: %d\n", *p); 
	
	//Look for empty space array
	int i; 
	for( i = 0; i < semBufLength; ++i ){

		//if(shmptr->semBuffer[i] != NULL){
		//
		//		shmptr->semBuffer[i] = p; 
		//}
	}
}
