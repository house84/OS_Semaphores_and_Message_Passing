/* 
 * Author: Nick House
 * Project: Semaphores and Message Passing
 * Course: CS-4760 Operating Systems, Spring 2021
 * File Name: driver.h
 */

#ifndef DRIVER_H
#define DRIVER_H

#include "shared.h"

void signalHandler(int); 							//Signal Handler 
void setTimer(int); 									//Start Timer
void setSHMemory(); 									//Initialize Shared Memory
void freeSHMemory(); 									//Detatch & Delete Shared Memory
void setSHMSem(); 									  //Initialize Sem Memory Resources
void freeSHMSem(); 										//Release Memory For Sem
void allocatePidArr(int);             //Set Memory Size of pidArr
void spawn(int, int);                 //Spawn Child (0=Producer 1 = Consumer)
void initializeBuffer();              //Initialize buffer to Null;
void initializeLogfile(); 						//Logfile Set up
bool greaterThan(int, int);           //Is arg 1 greater than arg 2
FILE *logfilePtr;                     //Open Logfile
extern int m; 												//Number of Producers
extern int n; 												//Number of Consumers
extern int myTimer; 									//Default timer value

#endif
