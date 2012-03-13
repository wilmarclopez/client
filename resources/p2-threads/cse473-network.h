#ifndef CSE473_NETWORK_INCLUDED

/**********************************************************************

   File          : cse473-network.h

   Description   : This is the network interfaces.

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
#define PROTOCOL_PORT 9157

#if defined(sun)
#define	INADDR_NONE		((in_addr_t) 0xffffffff)
#endif

/*  Functional Prototypes */

/**********************************************************************

    Function    : connectClient
    Description : connnect a client to the server
    Inputs      : addr - the address ("a.b.c.d/port")
    Outputs     : file handle if successful, -1 if failure

***********************************************************************/
int connectClient( char *addr );

/**********************************************************************

    Function    : serverConnect
    Description : connnect a server socket to listen for
    Inputs      : none
    Outputs     : file handle if successful, -1 if failure

***********************************************************************/
int serverConnect( void );

/**********************************************************************

    Function    : serverAccept
    Description : get a new connection on the server socket
    Inputs      : sock - server socket
    Outputs     : file handle if successful, -1 if failure

***********************************************************************/
int serverAccept( int sock );

/**********************************************************************

    Function    : recvData
    Description : receive data from the socket
    Inputs      : sock - server socket
                  blk - block to put data in
                  sz - maxmimum size of buffer
                  minsz - minimum bytes to read
    Outputs     : bytes read if successful, -1 if failure

***********************************************************************/
int recvData( int sock, char *blk, int sz, int minsz );

/**********************************************************************

    Function    : sendData
    Description : send  data to the socket
    Inputs      : sock - server socket
                  blk - block to put data in
                  len - length of data to send
    Outputs     : 0 if successful, -1 if failure

***********************************************************************/
int sendData( int sock, char *blk, int len );

#define CSE473_NETWORK_INCLUDED
#endif
