/*Author: Nick House
 * Project: Semaphores and Message Passing
 * Course: CS-4760 Operating SYstems, Spring 2021
 * File Name: monitor.c
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include "shared.h"
#include "monitor.h"

//===== Initialize Variables for Shared Memory  ====//
//key_t key;                                   //shm key
//size_t memSize;                              //Variable to hold memsize
int shmid;                                   //hold shmid
struct sharedMemory *shmptr;                 //Shared Memory Pointer

FILE *logfilePtr;                            //Pointer 
char logfile[100];                           //Logfile name

//==== Variables for Sem ====//
struct sembuf sops;                          //sem struct for semops
//key_t semKey;                                //Semaphore Key
int shmidSem;                                //Sem id

time_t t;                                    //Variable for time



void logEvent(); 
int * consumeProduct(); 
int addProdcut(); 
static void semWait(); 
static void semSignal(); 


//Set Shared Mem Resouces
void setShared(int myshmid, int myshmidSem){

	shmid = myshmid; 
	shmptr = (struct sharedMemory *) shmat(shmid, NULL, 0); 
	shmidSem = myshmidSem; 

}

//Free shmptr
void freeShmPtr(){

	shmdt(shmptr); 
}



//Produce Product
int * produce(pid_t pid, int idx, int myshmid, int myshmidSem){

	setShared(myshmid, myshmidSem);           //Set Shm
	
	fprintf(stderr,"Produer %d PID: %d entered monitor\n", idx, pid); 

	int x;                                   
	x = makeRandom(100);                      //Generate Random For Product
	int *xptr; 																//Creat ptr
	xptr = &x;                                //int -> ptr
	semWait(availableSpace);                  //Wait for Space in Buffer
	semWait(mutex);                           //Wait for Mutual Exclusion
	//addProduct(xptr);                         //Add x ptr -> buffer
	logEvent(pid, idx, producer);             //Log event
	semSignal(mutex);                         //Give up Mutual Exclusion
	semSignal(availableProducts);             //Signal Product Count
	freeShmPtr();                             //Free SHM Pointer

	fprintf(stderr,"Producer %d PID: %d leaving monitor\n", idx, pid); 

	return xptr;  
}


//Produce Product 
int addProduct(int * x){

	int i; 
	for( i = 0; i < semBufLength; ++i){

		if(shmptr->semBuffer[i] == NULL){

			shmptr->semBuffer[i] = x; 

			return 0; 
		}
	}

	return -1; 
}

	
//Consume Product 
int * consume(pid_t pid, int idx, int myshmid, int myshmidSem){

	setShared(myshmid, myshmidSem);           //Set Shm
	
	fprintf(stderr, "Consumer %d PID: %d entered monitor\n", idx, pid); 

	semWait(availableProducts);              //wait for products
	semWait(mutex);                          //wait for mutual exclusion
	int *x; 
	//x = consumeProduct();                    //Consume Product
	logEvent(pid, idx, consumer);            //Log event
	semSignal(mutex);                        //Release Mutual Exclusion
	semSignal(availableSpace);               //Increment avaialbleSpace
	freeShmPtr();                             //Free SHM Pointer

	fprintf(stderr,"Consumer %d PID: %d leaving monitor\n", idx, pid); 

	return x; 
}

//Consume Variable at address
int * consumeProduct(){
	
	int i; 
	int *x; 

	for( i = 0; i < semBufLength; ++i ){

		if( shmptr->semBuffer[i] != NULL ){

				x = shmptr->semBuffer[i]; 
				shmptr->semBuffer[i] = NULL; 

				return x; 
		}
	}
	
	i = -1; 
	x = &i; 

	return x; 
} 


//Wait Function Decrament and wait
static void semWait(int sem){

	printf("SemWait int: %d\n", sem); 
}


//Signal Function increment
static void semSignal(int sem){

	printf("semSignal int: %d\n", sem); 

}


//Create Random Number from 1-upper 
int makeRandom(int upper){

	srand(time(0)); 
	int num = (rand()%(upper -2)) + 1;  

	return num; 
}


//=== logEvent
void logEvent(pid_t pid, int idx, int who){

	char myWho[9]; 
	
	//if who == 0 producer, if who == 1 consumer
	if(who == 0){
		
		strcpy(myWho,"Producer"); 
	
	}	else{

		strcpy(myWho,"Consumer"); 
	}

	openLogfile(); 
		
	//fprintf(stderr, "%s: %d PID: %d in LogEvent\n", myWho, idx, pid); 

	fprintf(logfilePtr, "%s: %d PID: %d in LogEvent\n", myWho, idx, pid); 

	closeLogfile(); 

}


//=== Open Logfile
void openLogfile(){

	fprintf(stderr, "Logfile Name: %s\n", shmptr->logfile); 

	//Set ptr and Create/Open File append/read
	logfilePtr = fopen(shmptr->logfile, "a+"); 

	//Err Handling
	if( logfilePtr == NULL ){

		perror("monitor: ERROR: Failed to open logfile "); 
		exit(EXIT_FAILURE); 
	
	}
}

//=== Close LogFile
void closeLogfile(){

	fclose(logfilePtr); 

}
