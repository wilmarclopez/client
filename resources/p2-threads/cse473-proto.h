#ifndef CSE473_PROTO_INCLUDED

/**********************************************************************

   File          : cse473-proto.h

   Description   : This file contains the communication protocol function
                   prototypes.

   Last Modified : Jan 20 12:11:18 EST 2008
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

/* Defines */
#define MAX_BLOCK_SIZE 8096
#define REQ_COUNT      7

/* Data Structures */

/* This is the message type */
typedef enum {
     CLIENT_REQUEST,            /* message 1 - start exchange */
     SERVER_RESPONSE,           /* message 2 - server response */ 
     DH_FILE_XFER_INIT,         /* message 3 - initialize transfer */
     DH_FILE_XFER_BLOCK,        /* message 4 - transfer file block */
     DH_FILE_XFER_EXIT,         /* message 5 - exit the protocl */
     DH_CLIENT_Y_EXCHANGE,      /* message 6 - client asks for Y value */
     DH_SERVER_Y_RESPONSE,      /* message 7 - server provides Y value */
     DH_CLIENT_X_EXCHANGE,      /* message 8 - client sends X value */
     DH_SERVER_X_RESPONSE,      /* message 9 - server constructs key from client's X value */
} DhProtoMessageType;

typedef enum {
     DH_DECRYPT,                /* decrypt flag for transfer */ 
     DH_ENCRYPT,                /* encrypt flag for transfer */ 
} DhTransMessageType;

/* This is the message header */
typedef struct {
     unsigned short    msgtype;  /* message type */
     unsigned short    length;   /* message length */
} DhProtoMessageHdr;

/* server identifier -- for returned file */
char id[256];
extern int value;

/* Functional Prototypes */

/**********************************************************************

    Function    : waitMessage
    Description : wait for specific message type from the socket
    Inputs      : sock - server socket
                  hdr - the header structure
                  block - the block to read
                  my - the message to wait for
    Outputs     : bytes read if successful, -1 if failure

***********************************************************************/
extern int waitMessage( int sock, DhProtoMessageHdr *hdr, 
                 char *block, DhProtoMessageType mt );

/**********************************************************************

    Function    : sendMessage
    Description : Send message to other party
    Inputs      : sock - other's socket
                  hdr - message structure
                  block - message contents
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/
extern int sendMessage( int sock, DhProtoMessageHdr *hdr, char *block );

/**********************************************************************

    Function    : clientReq
    Description : this is the main function to execute the client
    Inputs      : address - address of the server
                  rate - request rate
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/
extern int clientReq( char *address, int rate ) ;

/**********************************************************************

    Function    : serverReq
    Description : this is the main function to execute the server
    Inputs      : none
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/
extern int serverReq( void );


/**********************************************************************

    Function    : serverInitThreads
    Description : Initialize the thread state for the server
    Inputs      : pool_size - number of server threads
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/
extern int serverInitThreads( int poolSize );


/**********************************************************************

    Function    : serverAddWork
    Description : Send the request to the processing implementation
    Inputs      : sock -- socket in which to send reply
                  req -- specification of work request
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/
extern int serverAddWork( int sock, char *req );

/**********************************************************************

    Function    : serverCleanup
    Description : Finalize the thread state for the server
    Inputs      : none
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/
extern int serverCleanup( void );


#define CSE473_PROTO_INCLUDED
#endif
