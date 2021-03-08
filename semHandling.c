/*Author: Nick House
 * Project: Semaphores and Message Passing
 * Course: CS-4760 Operating SYstems, Spring 2021
 * File Name: semHandling.c
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include "shared.h"


//Wait Function Decrament and wait
void semWait(int sem){

	printf("SemWait int: %d\n", sem); 
}


//Signal Function increment
void semSignal(int sem){

	printf("semSignal int: %d\n", sem); 

}


