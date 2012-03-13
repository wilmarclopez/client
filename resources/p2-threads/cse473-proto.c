/**********************************************************************

   File          : cse473-proto.c

   Description   : This is the higher level communication for client/server.

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

/**********************************************************************

    Function    : getMessage
    Description : receive data from the socket
    Inputs      : sock - server socket
                  hdr - the header structure
                  block - the block to read
    Outputs     : bytes read if successful, -1 if failure

***********************************************************************/

int getMessage( int sock, DhProtoMessageHdr *hdr, char *block )
{
     /* Read the message header */
     recvData( sock, (char *)hdr, sizeof(DhProtoMessageHdr), 
               sizeof(DhProtoMessageHdr) );
     hdr->length = ntohs(hdr->length);
     assert( hdr->length<MAX_BLOCK_SIZE );
     hdr->msgtype = ntohs( hdr->msgtype );
     
     // Display header	
	printf("Received message on socket %d with msgtype = %d"
		"and length = %d\n", sock, hdr->msgtype, hdr->length);
     
     if ( hdr->length > 0 )
        return( recvData( sock, block, hdr->length, hdr->length ) );
     return( 0 );
}

/**********************************************************************

    Function    : waitMessage
    Description : wait for specific message type from the socket
    Inputs      : sock - server socket
                  hdr - the header structure
                  block - the block to read
                  my - the message to wait for
    Outputs     : bytes read if successful, -1 if failure

***********************************************************************/

int waitMessage( int sock, DhProtoMessageHdr *hdr, 
                 char *block, DhProtoMessageType mt )
{
     /* Wait for init message */
     int ret = getMessage( sock, hdr, block );
     if ( hdr->msgtype != mt )
     {
        /* Complain, explain, and exit */
        char msg[128];
        sprintf( msg, "Server unable to process message type [%d != %d]\n", 
                 hdr->msgtype, mt );
        errorMessage( msg );
        exit( -1 );
     }

     /* Return succesfully */
     return( ret );
}

/**********************************************************************

    Function    : sendMessage
    Description : send data over the socket
    Inputs      : sock - server socket
                  hdr - the header structure
                  block - the block to send
    Outputs     : bytes read if successful, -1 if failure

***********************************************************************/

int sendMessage( int sock, DhProtoMessageHdr *hdr, char *block )
{
     int real_len = 0;

     /* Convert to the network format */
     real_len = hdr->length;
     hdr->msgtype = htons( hdr->msgtype );
     hdr->length = htons( hdr->length );
     if ( block == NULL )
          return( sendData( sock, (char *)hdr, sizeof(DhProtoMessageHdr) ) );
     else 
          return( sendData(sock, (char *)hdr, sizeof(DhProtoMessageHdr)) ||
                  sendData(sock, block, real_len) );
}


