#ifndef SHARED_H
#define SHARED_H 

struct sharedMemory {
  
	//===Arg Parameters===//
	
	int m; 								//Number of Procedures
	int n; 								//Number of Consumers
	int myTime;						//Seconds for Timer
	char logfile[100]; 		//Logfile Name
	
	
	//===Shared Memory Values===//
	
	int shmid;						//shm id
	pid_t *pidArray; 			//pointer to processes
	int totalProc; 				//Varaible to store number of processes


};


#endif
