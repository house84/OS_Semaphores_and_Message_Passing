/* 
 * Author: Nick House
 * Project: Semaphores and Message Passing
 * Course: CS-4760 Operating Systems, Spring 2021
 * File Name: main.h
 */

#ifndef MAIN_H
#define MAIN_H

void testPrint();											//For Dev Testing
void testSHM(); 											//For Dev Testing SHM
void signalHandler(int); 							//Signal Handler 
void setTimer(int); 									//Start Timer
void setSHMemory(); 									//Initialize Shared Memory
void freeSHMemory(); 									//Detatch & Delete Shared Memory
void openLogFile(); 									//Set Log File PTR Create File
void closeLogFile(); 									//Close Log File
void setSHMSem(int); 									//Initialize Sem Memory Resources
void freeSHMSem(); 										//Release Memory For Sem

extern int m; 												//Number of Producers
extern int n; 												//Number of Consumers
extern int myTimer; 									//Default timer value
extern struct sharedMemory *shmptr;   //Shmptr

#endif
