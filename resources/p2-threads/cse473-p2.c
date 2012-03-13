
/**********************************************************************

   File          : cse473-p2.c

   Description   : This is the main file for the client/server request handling.
                   (see .h for applications)

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

/* Project Include Files */
#include <cse473-util.h>
#include <cse473-proto.h>
#include <cse473-network.h>

/* Definitions */
#define ARGUMENTS "dr"
#define CLIENT_USAGE "USAGE: cse473-p2 <server  IP address> <request-rate>\n"
#define SERVER_USAGE "USAGE: cse473-p2-server <pool-size>\n"

int value = 0;  /* the collected value of the server -- simulate real work */

/**********************************************************************

    Function    : main
    Description : this is the main function for project #2
    Inputs      : argc - number of command line parameters
                  argv - the text of the arguments
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/

/* Functions */
int main( int argc, char **argv ) 
{
#ifndef THREAD_SERVER

    /* Check for arguments */
    if ( argc < 3 ) 
    {
        /* Complain, explain, and exit */
        errorMessage( "missing or bad command line arguments\n" );
        printf( CLIENT_USAGE );
        exit( -1 );
    }

    /* Check the address */
    if  ( inet_addr(argv[1]) == INADDR_NONE )
    {
        /* Complain, explain, and exit */
        char msg[128];
        sprintf( msg, "Bad server IP address [%.64s]\n", argv[2] );
        errorMessage( msg );
        printf( CLIENT_USAGE );
        exit( -1 );
    }

    strcpy(id, argv[1]);

    /* Now print some preable and get into the protocol, exit */
    printf( "Requests beginning [%s]\n", argv[0] );
    return( clientReq( argv[1], atoi(argv[2])) );

#else

    /* Check for arguments */
    if ( argc < 2 ) 
    {
        /* Complain, explain, and exit */
        errorMessage( "missing or bad command line arguments\n" );
        printf( SERVER_USAGE );
        exit( -1 );
    }

    /* initialize threads as necessary by implementation */
    serverInitThreads( atoi(argv[1]) );

     /* Just run the server */
    printf( "Preparing to serve requests [%s]...\n", argv[0] );
    serverReq();
    return( 0 );  /* never returns */

#endif
}
