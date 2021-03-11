/*
 * Author: Nick House
 * Project: Semaphores and Message Passing
 * Course: CS-4760 Operating Systems, Spring 2021
 * File Name: driver.c
 */

#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/ipc.h>
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
#include "driver.h"
#include "monitor.h"


int main(int argc, char *argv[]){

	//Initialize signal handling for CTRL+C
	signal(SIGINT, signalHandler); 

	//Set logfile Default 
	memset(logfile, '\0', sizeof(logfile)); 
	strcpy(logfile, "logfile");  
	
	//Parse input Args
	int c = 0; 

	while((c = getopt(argc, argv, "ho:p:c:t:")) != -1){

		switch(c) {

				case 'h':	//Print Help/Usage then Exit
									printf("\n//=== %s Usage Page ===//\n", argv[0]); ; 
									printf("\n%s [-h] [-o logfile] [-p m] [-c n] [-t time]\n", argv[0]); 
									printf("\n%s -h \t\t\t-> This Usage Page\n", argv[0]);  
									printf("%s -o [logfileName]\t-> Name of the file to save logs; default: logfile\n", argv[0]); 
									printf("%s -p m\t\t\t-> Number of producers; default m = 2\n", argv[0]); 
									printf("%s -c n\t\t\t-> Number of consumers; default n = 6\n", argv[0]); 
									printf("%s -t time\t\t-> Time (sec) after which the process will terminate; default: 100\n\n", argv[0]); 

									exit(EXIT_SUCCESS); 
									break; 
				
				case 'o': //Create Logfile Name Pointer default = logfile
									memset(logfile, '\0', sizeof(logfile)); 
									strcpy(logfile, optarg); 
									break; 

				case 'p': //Determine Number of Producers default m = 2
									m = atoi(optarg);
									break; 

				case 'c': //Number of Consumers default n = 6
									n = atoi(optarg); 
									break; 

				case 't': //Set program termination timer default mytime = 100
									myTimer = atoi(optarg);
									break; 
									
				case ':': //Arg Required
									fprintf(stderr, "Arguments Required, see usage [-h]\n");
									exit(EXIT_FAILURE); 
									break; 

				default:  //Err Message and Exit
									fprintf(stderr, "Invalid Argument, see usage [-h]\n"); 
									exit(EXIT_FAILURE); 
		}
	}


	//Initialize Timer
	setTimer(myTimer); 
	
	//Check Arg Parameters
	if( m < 1 || n < 1 || myTimer < 1 ){

		fprintf(stderr,"All Args must be a positive integer greater than Zero\n");
		fprintf(stderr,"Args: Producers: %d  Consumers: %d  Time: %d  Logfile: %s\n", m, n, myTimer, logfile); 
		
		exit(EXIT_FAILURE); 
	
	}


	//=== Check if There are more Producers than Consumers
	if( greaterThan(m, n) == true ){

		fprintf(stderr,"There must be more Consumers than Producers.\n"); 
		fprintf(stderr,"User passed: Consumers: %d  Producers: %d\n", n, m); 
		
		exit(EXIT_FAILURE); 
	} 


	//=== Check for too many producers
	if( m > 18 ){

		fprintf(stderr,"Too many Producers, in order to allow enough Consumer processes\n");		
		fprintf(stderr,"There must be no more than 18 producers created and running concurrently\n"); 

		exit(EXIT_FAILURE); 
	
	}


	//Set Shared Memory 
	setSHMemory();  
	
	//Set Sem
	setSHMSem(); 

	//Set Consumed Iterator
	shmptr->consumed = 0; 

	//Set produce flag
	shmptr->produce = true; 

	//Add logfile Name to ShMemory
	strcpy(shmptr->logfile, logfile);

	//Initialize logfileptr
//	initializeLogfile(); 

	//Initialize Buffer Array for Producers and Consumers
	void initializeBuffer(); 
	
	//Allocate pidArr space
	allocatePidArr(m+n); 


//============ Create Child Processes =============//
//*************************************************//

	//Create Producer Processes
	int i; 
	for( i = 0; i < m; ++i ){

		//spawn(idx, type)
		spawn(i+1, producer);
	
	}

	int j = 0;  
	
	//Create Consumer Processes Limited by 
	while( j < n){
 

		if(i == 19){

			wait(NULL); 
			--i; 
		}

		spawn(j+1, consumer); 

		++i; 
		++j; 
	} 


	//Allow Child Processes for testing
	while(wait(NULL)>0){

		if(shmptr->consumed == n){

			shmptr->produce = false; 
//			freeProducers(); 

			while(wait(NULL)>0); 	
		  break; 
		}
	} 
	
	
 	signalHandler(3126); 
	
	//Free Shared Memory
//	freeSHMemory(); 

	//Free Sem
//	freeSHMSem(); 

	return 0; 

}
