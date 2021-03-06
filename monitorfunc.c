/*
 * Author: Nick House
 * Project: Semaphores and Message Passing
 * Course: CS-4760 Operating Systems, Spring 2021
 * File Name: monitorfunc.c
 */

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
#include "monitor.h"


//=== Variables for Memory Allocation ===//
key_t key; 												//shm Key
size_t memSize; 									//Variable to hold shm size
int shmid; 												//shm id
struct sharedMemory *shmptr; 			//Shared Memory Resource
pid_t *pidArray; 									//Array of Processes
int totalProc = 0; 								//Variable to Store processes

//=== Program Parameter Variables ===//
int m = 2; 												//Default Number of Producers
int n = 6; 												//Default Number of COnsumers
int myTimer = 100; 								//Default time before Program Termination
char logfile[100]; 								//Default Char Arr for logfile
FILE *logFilePtr; 								//Pointer for Logfile Name


//=== Shared Local Variables ===//
static struct itimerval timer; 		//Set Global Timer Struct





//======================= Dev TESTING FUNCS =========================//

//=== Test Func Print Variables ===//
void testPrint(){

	printf("Logfile: %s  m: %d  n: %d  Timer: %d\n", logfile, m, n, myTimer); 

}


//Test SH Memory
void testSHM(){

	fprintf(stderr, "SHM X: %d  SHM Logname: %s\n", shmptr->x, shmptr->logFileName); 

}

//++++++++++++++++++++++++END DEV FUNC TEST++++++++++++++++++++++++++//




//***********************************************************//
//****************** Monitor Functions **********************//
//***********************************************************//


//=== Set Program Termination Timer ===//
void setTimer(int t){

	//Iniitialize signal handling 
	//for Timer Signal
	signal(SIGALRM, signalHandler); 

	//Set timer to Default or 
	//User specified Time
	timer.it_value.tv_sec = t; 
	timer.it_value.tv_usec = 0; 
	timer.it_interval.tv_sec = 0; 
	timer.it_interval.tv_usec = 0; 

	//Error Checking
	if( setitimer( ITIMER_REAL, &timer, NULL ) == -1 ){

			perror("monitor: ERROR: Timer Initialization Failure "); 
			exit(EXIT_SUCCESS); 
	}
}


//=== Set Logfile Pointer and Create File ===//
void openLogFile(){

	//Set PTR & Create/Open File Append/Read
	logFilePtr = fopen(logfile, "a+"); 

	//Err Check 
	if( logFilePtr == NULL ){

		perror("monitor: ERROR: Failed to open/create Logfile "); 
		exit(EXIT_FAILURE); 
	
	}
}


//=== Close LogFile ===//
void closeLogFile(){

	fclose(logFilePtr); 

}


//=== Initialize Shared Memory Resources ===//
void setSHMemory(){
	
	//Generate key
	key = ftok("Makefile", 'a'); 
	
	//Variable for easy adjust Dynamic SHMemory Size 
	memSize = sizeof(struct sharedMemory); 

	//Allocate Memory and Set ID
	shmid = shmget(key, sizeof(memSize), IPC_CREAT | S_IRUSR | S_IWUSR); 

	//Error Handling
	if( shmid == -1 ){

		perror("monitor: ERROR: Failed shmget() "); 
		exit(EXIT_FAILURE); 
	
	}


	//Attatch Memory Location to Pointer
	shmptr = (struct sharedMemory*) shmat(shmid, NULL, 0); 

}


//=== Detatch and Delete Shared Memory ===//
void freeSHMemory(){
	
	//Detatch SHM Pointer & Error Check
	if( shmdt( shmptr ) == -1 ){

		perror("monitor: ERROR: Failure Detatching memory ptr shmdt() "); 
		exit(EXIT_FAILURE); 
	
	}

	
	//Destroy SHM Id
	if( shmctl(shmid, IPC_RMID, NULL) == -1 ){

		perror("monitor: ERROR: Failure Destroying and Detatching shmid shmctc() "); 
		exit(EXIT_FAILURE); 

	}
}


//=== Terminate Processes and Free Memory if Signal initiated ===//
void signalHandler(int sig){

	//Check for Signal Type
	if( sig == SIGINT ){

		fprintf(stderr, "\nProgram Terminated by User\n"); 
	
	}else{

		fprintf(stderr, "\nProgram Terminated due to Timer\n"); 

	}


	//Detatch & Delete SHMemory
	freeSHMemory(); 

	
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

