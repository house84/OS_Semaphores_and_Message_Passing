/*
 * Author: Nick House
 * Project: Semaphores and Message Passing
 * Course: CS-4760 Operating Systems, Spring 2021
 * File Name: driverfunc.c
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
#include "driver.h"
#include "monitor.h" 


//=== Variables for Shared Memory Among Child Processes ===//
key_t key;                        //shm Key
size_t memSize;                   //Variable to hold shm size
int shmid;                        //shm id
pid_t *pidArray;                  //Pid of All Processes 
pid_t *consumerArray;             //Array of Consumer Processes
pid_t *producerArray;             //Array of Producer Processes
int consumerProc = 0;             //Variable to Store processes
int totalProc = 0; 
int pidCount = 0; 
int consumerCount = 0; 
int producerCount = 0; 

//=== Shared Memory Variables for Semaphores ===//
key_t semKey;                     //Semaphore Key
int shmidSem;                     //shm id for Semaphore
struct sembuf sops;               //sem struct

//=== Program Parameter Variables ===//
int m = 2;                        //Default Number of Producers
int n = 6;                        //Default Number of COnsumers
int myTimer = 100;                //Default time before Program Termination
FILE *logfilePtr;                 //Pointer for Logfile Name
time_t t;                         //Time variable


//=== Local Variables ===//
static struct itimerval timer;    //Set Global Timer Struct
bool flag = false;                //Flag to temp block Termination 
bool sigFlag = false;             //Flag to indicate termination begun


//***********************************************************//
//****************** Driver  Functions **********************//
//***********************************************************//

//initiate Logfile Pointer
void initializeLogfile(){

	logfilePtr = fopen(shmptr->logfile, "a+");
	shmptr->logfilePtr = logfilePtr; 

	fclose(shmptr->logfilePtr); 
}


//====Check if Producers (m)  is Greater than Consumers (n)
bool greaterThan(int m, int n){

	if(m >= n){

		return true; 
	}
	else{ 
		
		return false; 
  }
}

//==== Allocate Space for process Arrays ====//
void allocatePidArr(int size){
	
	pidArray = (int*) malloc(size*sizeof(int)); 
	
	consumerArray = (int*) malloc(n*sizeof(int)); 

	producerArray = (int*) malloc(m*sizeof(int));

	shmptr->firstEntry = false; 

	totalProc = n+m; 

}



//==== Create Shared Mem Buffer ====//
void intializeBuffer(){

	int i; 
	for(i = 0; i < semBufLength; ++i){

		shmptr->semBuffer[i] = 0;

	}
}

//==== Spawn Child Process ====//
void spawn(int idx, int type){
 

	//Check for Termination Signal Recieved
	if( sigFlag == true ) { return; }

	//Initialize Variable
	pid_t process_id; 

	//Create Child Process
	if((process_id = fork()) < 0){

		perror("driver: ERROR: failed to create proccess fork() "); 
		exit(EXIT_FAILURE); 

	}
	

	if( process_id == 0 ){ 
		
		//Temp Block Signal Handler from Terminating
		flag = true; 
	
		//add Child id to array 
		pidArray[pidCount] = process_id;  
		process_id; 
		++pidCount; 

		//Release Block
		flag = false; 

		char buffer_idx[10];
		sprintf(buffer_idx, "%d", idx); 

		char buffer_shmid[50];
		sprintf(buffer_shmid, "%d", shmid); 

		char buffer_shmidSem[50]; 
		sprintf(buffer_shmidSem, "%d", shmidSem); 


		if( type == producer ){

			if(execl("./producer", "producer", buffer_idx, buffer_shmid, buffer_shmidSem, (char*) NULL) == -1){

				perror("driver: ERROR: Failed to create Producer, execl() "); 
				exit(EXIT_FAILURE); 
			}
			
			producerArray[producerCount] = process_id; 
			++producerCount;  
		}
		
		else{

			if(execl("./consumer", "consumer", buffer_idx, buffer_shmid, buffer_shmidSem, (char*) NULL) == -1){

				perror("driver: ERROR: Failed to create Consumer, execl() "); 
				exit(EXIT_FAILURE); 
			}
			
			consumerArray[consumerCount] = process_id; 
			++consumerCount; 
		}
	  
		exit(EXIT_SUCCESS); 
	}
}


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


//=== Initialize Semaphore Resources ===//
void setSHMSem(){

	//Set SemKey
	if(( semKey = ftok("driver.c", 'A')) == -1){
			
			perror("monitor: ERROR: Failure to generate semKey ftok() ");
			exit(EXIT_FAILURE); 
	}

	if((shmidSem = semget(semKey, 4, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR )) == -1){

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
	if((semctl(shmidSem, availableSpace, SETVAL, semBufLength)) == -1){
			
			perror("monitor: ERROR: Failed to create semaphore semctl() ");
			exit(EXIT_FAILURE); 
		}
	
	//Semaphore 2-availalbeProducts is initialized to zero 
	if((semctl(shmidSem, availableProducts, SETVAL, 0)) == -1){
			
			perror("monitor: ERROR: Failed to create semaphore semctl() ");
			exit(EXIT_FAILURE); 
		}

	//Semaphore 3- completeConsumers is initialized to number of consumers: n
	if((semctl(shmidSem, completeConsumers, SETVAL, n+2)) == -1){

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

	//set sigFlag to stop any further child processes
	sigFlag = true; 

	openLogfile(); 

	time(&t); 
	
	//Check for Signal Type
	if( sig == SIGINT ){

		fprintf(stderr, "\nProgram Terminated by User\n"); 
	
		fprintf(shmptr->logfilePtr, "\nTime: %sProgram Terminated by User\n", ctime(&t)); 
	
	}else if( sig == 3126 ){

		fprintf(stderr, "\nAll Items Have been Consumed\n"); 
	
		fprintf(shmptr->logfilePtr, "\nTime: %sAll Items Have been Consumed\n", ctime(&t)); 
	
	}else {
		
		fprintf(stderr, "\nProgram Terminated due to Timer\n"); 

		fprintf(shmptr->logfilePtr, "\nTime: %sProgram Terminated due to Timer\n", ctime(&t)); 
	}
	
	fprintf(shmptr->logfilePtr,"\n//************** END FILE ENTRY **************//\n\n"); 

	closeLogfile(); 
	//Allow Potential Process to add PID to array
	while(flag == true){}

	//Free Sem List
	freeSHMSem(); 
	
	//Detatch & Delete SHMemory
	freeSHMemory();

	if( sig == 3126 ){ return; } 
	
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

