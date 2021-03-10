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
#include <stdbool.h> 
#include "shared.h"
#include "monitor.h"		


//Pass as args shmid, shmidSem, 
int main(int argc, char *argv[]){
	
	pid_t p = getpid();                   //Get Process PID
	int idx = atoi(argv[1]);              //Get index
	int shmid = atoi(argv[2]);            //Get shmid
	int shmidSem = atoi(argv[3]);         //Get shmidSem
	
	fprintf(stderr,"Producer %d, PID: %d has been created\n", idx, p); 

	while(true){                          //Always be producing 

		produce(p, idx, shmid, shmidSem);     //Call Produce in Monitor
		
		fprintf(stderr,"Producer %d, PID: %d is going to sleep\n", idx, p); 

		sleep(makeRandom(5));              //Sleep for random 1-5 Seconds

		fprintf(stderr,"Producer %d, PID: %d has woken up\n", idx, p); 

	}
   
	exit(EXIT_SUCCESS); 

}
