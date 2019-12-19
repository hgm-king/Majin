#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>
#include "server.h"
#include "epoll.h"

int server_write( struct epoll_event *events, int i )  {
  if ( !events || i < 0 )  { return -1; }

  char *msg = "HTTP/1.1 200 OK\r\nDate: Mon, 27 Jul 2009 12:28:53 GMT\r\nServer: Apache/2.2.14 (Win32)\r\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\nContent-Length: 51\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\n<h1>What it do baby</h1><h4>Sara b is a baddie</h4>";
  int len = strlen(msg);
  int rc = send(events[i].data.fd, msg, len, 0);

  if (rc < 0) {
    fprintf( stdout, "send() failed \n" );
    return -1;
  }

  //fprintf( stdout, "%d bytes sent in response\n", len );
  return 0;
}
