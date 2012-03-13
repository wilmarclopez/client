
/**********************************************************************
   File          : cse473-client.c

   Description   : General client files

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

int std_requests[] = {2, 10, 3, 3, 8, 10, 1, 1, 1, 6};  /* just requests */
int stop_requests[] = {2, 10, 3, 3, 8, 10, 1, 1, 6, 0};    /* include terminate */
int term_requests[] = {2, 10, 3, 3, 8, 10, 1, -10, 6, 0};    /* include terminate req */
int requests[] = {2, 10, 3, 3, 8, 10, 1, -10, 6, 0};    /* term-requests */
int req_index = 0;

typedef struct client_work {
  char address[MAX_BLOCK_SIZE];
  int index;
} client_work_t;

/* Functional Prototypes */

/* 

  CLIENT FUNCTIONS 

*/

#ifndef THREAD_SERVER
/**********************************************************************

    Function    : clientGenReq
    Description : generate request for the server
    Inputs      : req - location for request string
                  index - index into request table 
    Outputs     : bytes in request if successful, -1 if failure

***********************************************************************/

int clientGenReq( char *req, int index )
{
  sprintf(req, "%d", requests[index]);
  if ( requests[index] <= 0 ) sleep(2); /* delay for special tasks */

  /* Return succesfully, number of bytes */
  return ( 1 );
}

/**********************************************************************

    Function    : sendClientReq
    Description : Generate and send request to server
    Inputs      : arg - index for generating request 
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/

void *sendClientReq( void* arg ) 
{
     /* Local variables */
     DhProtoMessageHdr hdr;
     char req[MAX_BLOCK_SIZE];
     int bytes;
     char block[MAX_BLOCK_SIZE];
     int sock;
     client_work_t *work = (client_work_t *)arg;

     printf( "client[0x%x]: start thread - index %d\n", (unsigned) pthread_self(), work->index );

     /* connect to server */
     sock = connectClient( work->address );

     /* generate req string */
     bytes = clientGenReq( req, work->index );

     /* Send the init data */
     hdr.msgtype = CLIENT_REQUEST;
     hdr.length = strlen(req)+1;
     printf( "\nclient[0x%x]: before send (req %d) - index %d/client sock %d\n", 
	     (unsigned) pthread_self(), requests[work->index], work->index, sock );
     sendMessage( sock, &hdr, req );
     
     /* Send the ack, wait for server ack */
     waitMessage( sock, &hdr, block, SERVER_RESPONSE );
     printf( "client[0x%x]: after recv (req %d) - index %d/client sock %d\n", 
	     (unsigned) pthread_self(), requests[work->index], work->index, sock );

     close( sock );
     free( work );
     pthread_exit( NULL );
}


/**********************************************************************

    Function    : clientReq
    Description : this is the main function to execute the protocol
    Inputs      : fname - filename of the file to transfer
                  address - address of the server
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/

int clientReq( char *address, int reqRate ) 
{
    /* Local variables */
    pthread_attr_t attr;
    pthread_t tid[REQ_COUNT];
    client_work_t *work;
    void *status;
    int i, rc;
    
    /* initialize attributes for both threads */
    pthread_attr_init( &attr );
    pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE );

    for ( i = 0; i < REQ_COUNT; i++ ) {
      work = (client_work_t *) malloc(sizeof(client_work_t));
      strcpy(work->address, address);
      work->index = i;

      sleep( reqRate );
      /* create client thread */
      rc = pthread_create( &tid[i], &attr, sendClientReq, (void *)work );
      if (rc) {
	errorMessage( "clientReq: failed in pthread_create\n" );
	exit( -1 );
      }
    }

    pthread_attr_destroy(&attr);
    for ( i = 0; i < REQ_COUNT; i++ ) {
      /* join with the client workers */
      rc = pthread_join( tid[i], &status );
      if (rc) {
	errorMessage( "clientReq: failed in pthread_join\n" );
	exit( -1 );
      }
    }
    printf("clientReq: completed all %d threads\n", i);

    /* Return successfully */
    return( 0 );
}
#endif
