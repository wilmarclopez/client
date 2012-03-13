#include <cstdlib>
#include "CommunicationServices.h"
#include "Logger.h"
#include "threadpool.h"

#define POOLSIZE 50

CommunicationServices *comm = CommunicationServices::getInstance();
Logger *logger = Logger::getInstance();
ThreadPool tp(POOLSIZE);

int main(int argc, char **argv)
{
	if ( argc != 3 ) {
		logger->error("Usage <address> <port>");
		return -1;
	}
	
	int mainSocket = comm->connectClient(argv[1], atoi(argv[2]));
	tp.initializeThreads();
	
}
