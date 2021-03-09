/*
 * Author: Nick House
 * Project: Semaphores and Message Passing
 * Course: CS-4760 Operating Systems, Spring 2021
 * File Name: mainfunc.c
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
#include <sys/sem.h>
#include <sys/ipc.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include "shared.h"
//#include "monitor.h"


//=== Variables for Shared Memory Among Child Processes ===//
key_t key; 												//shm Key
size_t memSize; 									//Variable to hold shm size
int shmid; 												//shm id
struct sharedMemory *shmptr; 			//Shared Memory Resource
pid_t *pidArray; 									//Array of Processes
int totalProc = 0; 								//Variable to Store processes

//=== Shared Memory Variables for Semaphores ===//
key_t semKey; 										//Semaphore Key
int shmidSem; 										//shm id for Semaphore
struct sembuf sob;	 							//sem struct

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


//=== Initialize Semaphore Resources ===//
void setSHMSem(int bufferSize){

	//Set SemKey
	if(( semKey = ftok("monitor.c", 'A')) == -1){
			
			perror("monitor: ERROR: Failure to generate semKey ftok() ");
			exit(EXIT_FAILURE); 
	}

	if((shmidSem = semget(semKey, 3, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR )) == -1){

		perror("monitor: ERROR: Failed to semget() "); 
		exit(EXIT_FAILURE); 
	}
	

	//=== Create Semaphore ===// 

	//Semaphore 0-Mutex is  Mutual Exclusion (set to 1 when available and 0 when used)
	if((semctl(shmidSem, mutex, SETVAL, 1)) == -1){
			
			perror("monitor: ERROR: Failed to create semaphore semctl() ");
			exit(EXIT_FAILURE); 
		}
	
	//Semphore 1-availableSpace is Size of Buffer, space available for produced items
	if((semctl(shmidSem, availableSpace, SETVAL, bufferSize)) == -1){
			
			perror("monitor: ERROR: Failed to create semaphore semctl() ");
			exit(EXIT_FAILURE); 
		}
	
	//Semaphore 2-availalbeProducts is initialized to zero 
	if((semctl(shmidSem, availableProducts, SETVAL, 0)) == -1){
			
			perror("monitor: ERROR: Failed to create semaphore semctl() ");
			exit(EXIT_FAILURE); 
		}
}


//=== Free Semaphore Resources ===//
void freeSHMSem(){

	if(( semctl(shmidSem, 0, IPC_RMID)) == -1){

		perror("monitor: ERROR: Failed to release Sem memory semctl() ");
		exit(EXIT_FAILURE); 
	
	}
}


//=== Initialize Shared Memory Resources ===//
void setSHMemory(){
	
	//Generate Key
	if(( key = ftok("Makefile", 'a')) == -1){
			
			perror("monitor: ERROR: Failure to generate key ftok() ");
			exit(EXIT_FAILURE); 
	}
	
	//Variable for easy adjust Dynamic SHMemory Size 
	memSize = sizeof(struct sharedMemory); 

	//Allocate Memory and Set ID
	if((shmid = shmget(key, sizeof(memSize), IPC_CREAT | S_IRUSR | S_IWUSR))== -1){ 

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

	//Free SHM Semaphore
	void freeSHMSem();
	
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

