/**
 * Resources:
 * http://beej.us/guide/bgnet/output/html/multipage/index.html
 * https://computing.llnl.gov/tutorials/pthreads/
 * http://www.cse.psu.edu/~tjaeger/cse473-s08/cse473-s08-p2.html
 */

#include <cstdlib>
#include <pthread.h>
#include  <iostream>
#include  <cstdio>
#include "CommunicationServices.h"
#include "Logger.h"
#include "threadpool.h"

#define POOLSIZE 50
#define REQ_COUNT 5

using namespace std;

CommunicationServices *comm = CommunicationServices::getInstance();
Logger *logger = Logger::getInstance();
//ThreadPool tp(POOLSIZE);
int mainSocket;

void* sendRequest(void* param)
{
	MessageHeader *hdr = new MessageHeader(CLIENT_REQUEST, sizeof(int));
	int delay = (int) param;
	
	cout << "!WOW\n";
	
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	if ( argc != 3 ) {
		logger->error("Usage <address> <port>");
		return -1;
	}
	
	int rc;
	pthread_t thread[POOLSIZE];
	pthread_attr_t attr;
	
	mainSocket = comm->connectClient(argv[1], argv[2]);	
	
	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for(int i = 0; i<REQ_COUNT; i++) {
		//set this thread as joinable
		rc = pthread_create(&thread[i], &attr, sendRequest, (void *)i);
		
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	
	// free attribute 
	pthread_attr_destroy(&attr);
	
	// wait for the other threads */	
	void *status;
	for(int t=0; t<REQ_COUNT; t++) {
		//tell current thread to join these threads
		rc = pthread_join(thread[t], &status);
		if (rc) {
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
		printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
	}
	

	pthread_exit(NULL);
	
	//tp.initializeThreads();
	
}
