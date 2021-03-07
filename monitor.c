/*
 * Author: Nick House
 * Project: Semaphores and Message Passing
 * Course: CS-4760 Operating Systems, Spring 2021
 * File Name: monitor.c
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
				
				case 'o': //Create Logfile Name Pointer default = logile
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

	//Set Shared Memory 
	setSHMemory();  
	
	//Set Sem
	//setSHMSem(3); 
	
	//Set Logfile Pointer and Name
	openLogFile(); 

	//Close Logfile
	closeLogFile(); 

	//====TESTING SHARED MEMORY==========
	
				shmptr->x = 10; 
				strcpy(shmptr->logFileName, logfile); 

				//Test SHMemory
				testSHM(); 

				//Test Calling Library
				testPrint(); 
	
	//====END TESTING SHARED MEMORY========

	
	//Free Shared Memory
	freeSHMemory(); 

	//Free Sem
//	freeSHMSem(); 

	return 0; 

}
