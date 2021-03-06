//Create Functions for Monitor

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include "shared.h"


//=== Variables for Memory Allocation ===//
key_t key; 												//shm Key
size_t memSize; 									//Variable to hold shm size
int shmid; 												//shm id
struct sharedMemory *shmptr; 			//Shared Memory Resource
pid_t *pidArray; 									//Array of Processes
int totalProc = 0; 								//Variable to Store processes

int m = 2; 

void testPrint(int x){

	printf("Library int: %d\n", x); 

}


//==== Terminate Processes and Free Memory if Signal initiated ===//
void signalHandler(int sig){

	//Check for Signal Type
	if( sig == SIGINT ){

		fprintf(stderr, "\nProgram Terminated by User\n"); 
	
	}else{

		fprintf(stderr, "\nProgram Terminated due to Timer\n"); 

	}

	//===Detatch and Delete Shared Memory===//
	if(shmdt( shmptr ) == -1){

		perror("monitor: ERROR: shmptr() failure in Signal Handler "); 
		exit(EXIT_FAILURE); 

	}

	
	//===Delete shmid===//
	if(shmctl( shmid, IPC_RMID, NULL) == -1){

		perror("monitor: ERROR: shmctl() failure in Signal Handler "); 
		exit(EXIT_FAILURE); 

	}

	
	//===Terminate Child Processes===//
	int i; 
	for( i = 0; i < totalProc; ++i ){

		if(kill(pidArray[i], SIGKILL ) == -1 && errno != ESRCH ) {

				perror("monitor: ERROR: Failed to kill processes "); 
				exit(EXIT_FAILURE); 

		}
	}


	//===Destroy Zombies===//
	while( wait(NULL) != -1 || errno == EINTR ); 

	exit(EXIT_SUCCESS); 

}


	

