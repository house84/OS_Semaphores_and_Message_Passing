/*
 * Author: Nick House
 * Project: Semaphores and Messge Passing
 * Course: CS-4760 Operating Systems, Spring 2021
 * File Name: shared.h
 */

#ifndef SHARED_H
#define SHARED_H 

#include <semaphore.h>
#include <stdbool.h>
#define semBufLength 4									//set buffer Length for global Access

enum Sems{mutex, availableSpace, availableProducts, completeConsumers}; 
enum prod_cons{producer, consumer}; 
//mutex = 1   													Ensure Mutual Exclusion
//availableSpace = semBufLenth					Size of Buffer: Amount of space available to producer
//availableProducts = 0 								Amount of Products available to Consumer 		

struct sharedMemory {
	
	//Dev Testing Variables
	int x; 																//Testing Int
	int consumed; 												//Consumed Items
	bool firstEntry; 											//First Logfile Entry
	bool produce;                       //Check if all Consumers are complete
	char logfile[100];   								//Logfile Name
	FILE *logfilePtr; 										//Shared Logfile
	int semBuffer[semBufLength]; 				//memory Addresses for Products Initialize to NULL

};


#endif
