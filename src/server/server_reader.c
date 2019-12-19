#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>
#include "server.h"
#include "epoll.h"

int server_read( struct epoll_event *events, int i )  {

  int total, len, rc = 0;
  char buffer[80] = {'\0'};
  /* Receive all incoming data on this socket            */
  /* before we loop back and call poll again.            */
  do
  {
    /* Here we will check for now if we need to           */
    /* stop reading from the socket or not. For           */
    /* now we'll do it just by cutting off at a           */
    /* certain number of bits                             */
    if ( strstr( buffer, "\r\n\r\n" ) != NULL )  {
      return 1;
      // hopefully by breaking, it will not prevent
      // us from writing to the socket.
    }

    /* Receive data on this connection until the         */
    /* recv fails with EWOULDBLOCK. If any other         */
    /* failure occurs, we will close the                 */
    /* connection.                                       */
    rc = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
    if (rc < 0)  {
      fprintf(stderr, "read() failed: %s\n", strerror( errno ));
      if (errno != EWOULDBLOCK) {
        perror("  recv() failed");
        return -1;
      }
      return 0;
    }

    /* Check to see if the connection has been           */
    /* closed by the client                              */
    if (rc == 0)
    {
      printf("  Connection closed\n");
      return -1;
    }

    /* Data was received                                 */
    len = rc;
    total += len;
    //printf("  %d bytes received\n", len);

  } while ( 1 );

  return -1;
}
