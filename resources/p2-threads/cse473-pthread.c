
/**********************************************************************

   File          : cse473-pthread.c

   Description   : This is the pthreads implementation for the project

   Last Modified : Jan 20 09:54:33 EST 2008
   By            : Trent Jaeger

***********************************************************************/
/**********************************************************************
Copyright (c) 2008 The Pennsylvania State University
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of The Pennsylvania State University nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************/


/* Include Files */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

/* Project Include Files */
#include <cse473-util.h>
#include <cse473-proto.h>
#include <cse473-network.h>
#include <cse473-server.h>
#include <cse473-pthread.h>

/**********************************************************************

    Function    : setUncancelable
    Description : Set thread as beyond cancellation 
    Inputs      : tid - Thread id to remove from the ongoing queue
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/

int setUncancelable( pthread_t tid )
{
  /* Local variables */
  thread_ongoing_t *ongoing, *prev;

  /* remove itself from ongoing work */
  ongoing = tp->ongoing;
  prev = NULL;
  
  while ( ongoing != NULL ) {

    if ( ongoing->tid == tid ) {
      ongoing->no_cancel = 1;
      goto end;
    }

    ongoing = ongoing->next;
  }
  return -1;  /* not in ongoing? */

 end:
  return 0;
}


/**********************************************************************

    Function    : removeFromOngoing
    Description : Remove current thread from ongoing work 
    Inputs      : tid - Thread id to remove from the ongoing queue
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/

int removeFromOngoing( pthread_t tid ) 
{
  /* Local variables */
  thread_ongoing_t *ongoing, *prev;

  /* remove itself from ongoing work */
  ongoing = tp->ongoing;
  prev = NULL;
  
   while ( ongoing != NULL ) {

     if ( ongoing->tid == tid ) {

       if ( prev ) {
	 prev->next = ongoing->next;
       }
       else {
	 tp->ongoing = ongoing->next;
       }

       free(ongoing->work);
       free(ongoing);
       goto end;
     }

     prev = ongoing;
     ongoing = ongoing->next;
   }

   return -1;   /* thread not in ongoing? */

 end:
   return 0;
}


  /*
  
  Worker Functions 
  
  */

/**********************************************************************

    Function    : serverProcessReq
    Description : Pthreads version that performs request
    Inputs      : sock - socket to reply to client
                  req - client request message
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/

/* set to uncancelable work -- prevent cancellation that is too late */
/* YOUR CODE somewhere in the function setProcessReq below (#4D) */

int serverProcessReq( int sock, int arg ) 
{
     /* Local variables */
     DhProtoMessageHdr hdr;
     char block[MAX_BLOCK_SIZE];
     int delay;

     /* check for cancellation */
     printf("serverProcessReq[0x%x]: check terminate before work - server sock: %d; req: %d\n", 
	    (unsigned)pthread_self(), sock, arg );
     /* YOUR CODE HERE (#4D) */
     printf("serverProcessReq[0x%x]: no terminate before work - server sock: %d; req: %d\n", 
	    (unsigned)pthread_self(), sock, arg );


     /* Fake request processing with sleep */
     delay = arg;
     printf("serverProcessReq[0x%x]: do work - server sock: %d; req: %d\n", 
	    (unsigned)pthread_self(), sock, arg );
     printf("====== work: %d seconds =======\n", delay );
     sleep(delay);


     /* check for cancellation -- like to set as committed to finish after here */
     printf("--- serverProcessReq[0x%x]: check terminate after work - server sock: %d; req: %d\n", 
	    (unsigned)pthread_self(), sock, arg );
     /* YOUR CODE HERE (#4D) */
     printf("serverProcessReq[0x%x]: no terminate after work - server sock: %d; req: %d\n", 
	    (unsigned)pthread_self(), sock, arg );

     /* Build reply message */
     hdr.msgtype = SERVER_RESPONSE;
     hdr.length = 0;
     strcpy(block, "done");
     hdr.length += strlen(&block[hdr.length])+1;

     /* send the reply */
     printf("serverProcessReq[0x%x]: send response - server sock: %d; req: %d\n", 
	    (unsigned)pthread_self(), sock, arg );
     sendMessage( sock, &hdr, block );

     /* remove from the set of ongoing tasks */
     /* YOUR CODE HERE (#4C) */
  
     close(sock);
     return 0;
}


/**********************************************************************

    Function    : worker
    Description : function of each thread in the thread pool
    Inputs      : none
    Outputs     : NULL pointer

***********************************************************************/

void *worker( void *arg ) 
{
  int result = 0;
  thread_work_t *work = (thread_work_t *)arg;

  printf("+++ worker thread[0x%x]: server sock: %d; req: %d\n", 
	 (unsigned) pthread_self(), work->sock, work->arg );
  result = serverProcessReq( work->sock, work->arg );
  /* pthread_exit( NULL );  -- when it's a thread */
}  



  /*
  
  Main Thread Functions 
  
  */


/**********************************************************************

    Function    : terminateProcessReq
    Description : Stop all requests that correspond to argument 
    Inputs      : arg - request identifier
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/

int terminateProcessReq( int arg )
{
  /* Local variables */
  /* YOUR CODE HERE (#4B) */

  printf("terminateProcessReq: terminate case: %d\n", arg);

  /* find matching ongoing work */
  /* YOUR CODE HERE (#4B) -- print:
  printf("TerminateProcessReq: terminate thread: 0x%x: sock: %d; arg: %d\n", *** args ***
   * when a thread is selected for termination */

  /* cancel matches and send quick reply */
  /* YOUR CODE HERE (#4C) - print:
  printf("TerminateProcessReq: send reply on behalf of thread: 0x%x: sock: %d; arg: %d\n", *** args ***
  when a thread is actually deleted, and this thread send a reply on its behalf */

  return 0;
}


/**********************************************************************

    Function    : serverAddWork
    Description : Pthreads version to add work item to queue
    Inputs      : sock - socket to reply to client
                  req - client request message
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/

int serverAddWork( int sock, char *req ) 
{
  /* Local variables */
  thread_work_t *work;
  int arg = atoi(req);
  /* YOUR CODE HERE */

  work = (thread_work_t *)malloc(sizeof(thread_work_t));
  work->arg = arg;
  work->sock = sock;
  work->next = NULL;

  /* handle the normal request case 
    YOUR CODE HERE (#2) - print: 

    printf("=== serverAddWork: work for server sock %d: %d secs\n",
	   sock, arg );
   * when a normal request is made */
  if ( arg > 0 )  {  
    worker( (void *)work );  /* need to replace this with thread creation */
  }

  /* handle shutdown case 
     YOUR CODE HERE (#3) - print: 
     printf("=== serverAddWork: shutdown case: server sock: %d\n", sock );
     printf("serverAddWork: wait for other threads before shutdown: threads started: %d\n",
     tp->num_threads);
     printf("serverAddWork: shutdown: threads running after join: %d\n", (tp->ongoing ? 0 : 1));
   * for this case */

  else if ( arg == 0 ) {
    serverProcessReq( sock, arg ); 
    exit(0); 
  }

  /* handle thread termination case 
     YOUR CODE HERE (#4A) - print:
     printf("=== serverAddWork: terminate requests: %d\n", atoi(req));
   * when a request is made to terminate a thread */
  /* NOTE: for task #4C, store the new normal request in the tp->ongoing list 
     See the code for removeFromOngoing to get an idea for how to add to the list */

  else {
    serverProcessReq( sock, 0 ); 
  }

  /* End of setup for processing this request */
  printf("serverAddWork: finished setup work for server sock %d: %d\n", 
	 sock, arg );

  return 0;
}


/**********************************************************************

    Function    : serverInitThreads
    Description : Pthreads version to initialize pool of size poolSize
    Inputs      : poolSize - number of threads in the thread pool
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/

int serverInitThreads( int poolSize )
{
  /* Initialize the tpool_t structure as necessary */
  /* YOUR CODE HERE */

  return 0;
}
