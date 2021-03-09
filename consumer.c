/*
 * Author: Nick House
 * Project: Semaphores and Message Passing
 * Course: CS-4760 Operating Systems, Spring 2021
 * File Name: consumer.c
 */

#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "monitor.h"

int main(int argc, char *argv[]){

	pid_t c = getpid();             //Get Process ID
	int idx = atoi(argv[1]);        //Get Index of Consumer 
	int shmid = atoi(argv[2]); 			//Get shmid
	int shmidSem = atoi(argv[3]); 	//get shmidSem

	//Print Message to Terminal -> Consumer Created
	fprintf(stderr, "Consumer %d, PID: %d has been created\n", idx, c); 

	//Print Message Sleeping
	fprintf(stderr, "Consumer %d, PID: %d is going to sleep\n", idx, c); 

	//Sleep for random time 1-10 seconds
	sleep(makeRandom(10)); 

	//Print Wake Message
	fprintf(stderr, "Consumer %d, PID: %d has woken up\n", idx, c); 

	//Call Consume to get variable
	consume(c, idx, shmid, shmidSem); 

	//Print Program termination Message	
	fprintf(stderr, "Consumer %d, PID: %d Terminating\n", idx, c); 

	exit(EXIT_SUCCESS); 

}
