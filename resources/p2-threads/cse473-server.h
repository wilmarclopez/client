
/**********************************************************************

   File          : cse473-pthread.h

   Description   : This file contains the pthread prototypes

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

/* work structure */
typedef struct thread_work {
  int sock;
  int arg;
  struct thread_work *next;
} thread_work_t;


/**********************************************************************

    Function    : serverInitThreads
    Description : For Linux, Create on demand 
    Inputs      : poolSize - number of threads in the thread pool
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/
extern int serverInitThreads( int poolSize );


/**********************************************************************

    Function    : serverAddWork
    Description : Linux version, create a clone to service the request
    Inputs      : sock - socket to reply to client
                  req - client request message
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/
extern int serverAddWork( int sock, char *req );


/**********************************************************************

    Function    : serverProcessReq
    Description : Pthreads version that performs request
    Inputs      : sock - socket to reply to client
                  arg - indicator of operation
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/
extern int serverProcessReq( int sock, int arg );


