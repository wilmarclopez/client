/**********************************************************************

   File          : cse473-server.c

   Description   : Generic server functions

   Last Modified : Jan 20 12:11:18 EST 2008
   By            : Trent Jaeger

***********************************************************************/

/* Include Files */
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>

/* Project Include Files */
#include <pthread.h>
#include <cse473-util.h>
#include <cse473-network.h>
#include <cse473-proto.h>

/* Functional Prototypes */

/* 

  SERVER FUNCTIONS 

*/

#ifdef THREAD_SERVER

/**********************************************************************

    Function    : receiveReq
    Description : receive a request  over the wire
    Inputs      : sock - the socket to receive the file over
                  req - reference to request buffer
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/

int receiveReq( int sock, char **req ) 
{
     /* Local variables */
     unsigned long totalBytes = 0;
     DhProtoMessageHdr hdr;
     char block[MAX_BLOCK_SIZE];

     /* Receive the init message */
     waitMessage( sock, &hdr, block, CLIENT_REQUEST );
     totalBytes = hdr.length;
     *req = (char *) malloc(totalBytes);
     strcpy(*req, block);

     /* return successfully */
     return( totalBytes );
}

/**********************************************************************

    Function    : serverReq
    Description : this is the main function to execute the protocol
    Inputs      : none
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/

int serverReq( void )
{
    int server = 0, errored = 0, newsock = 0;
    fd_set readfds;

    server = serverConnect();

	//DEBUG
	int debug = 0;
	
    /* Repeat until the socket is closed */
    while ( !errored )
    {
		printf("\n%d",debug++);
		
         FD_ZERO( &readfds );
         FD_SET( server, &readfds );
         if ( select(server+1, &readfds, NULL, NULL, NULL) < 1 )
         {
             /* Complain, explain, and exit */
              char msg[128];
              sprintf( msg, "failure selecting server connection [%.64s]\n",
                       strerror(errno) );
              errorMessage( msg );
              errored = 1;
         }
         else
         {
			/* Accept the connect, receive request, and return */
			if ( (newsock = serverAccept(server)) != -1 )
			{
				char *req;

				if (receiveReq( newsock, &req ) < 0) errored = 1;
				serverAddWork( newsock, req );
				fflush( stdout );
			}
			else {
				/* Complain, explain, and exit */
				char msg[128];
				sprintf( msg, "failure accepting connection [%.64s]\n", 
					  strerror(errno) );
				errorMessage( msg );
				errored = 1;
			}
         }
    }

    /* Return successfully */
    return( 0 );
}

#endif
