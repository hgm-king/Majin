// Copyright (c) 2019 HG King Development and Design All Rights Reserved.
// ssocket.c file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssocket.h"


int socket_create( char *host, int port, int backlog )
{
  if ( 0 > port || 0 > backlog )  { return -1; }

  int listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
  if ( 0 > listen_sd )
  {
    perror( "socket() failed" );
    return -1;
  }

  if ( 0 > socket_setup( listen_sd ) )
  {
    perror( "socket setup failed" );
    return -1;
  }

  if ( 0 > socket_bind( listen_sd, host, port ) )
  {
    perror( "socket bind failed" );
    return -1;
  }

  if ( 0 > socket_listen( listen_sd, backlog ) )
  {
    perror( "socket bind failed" );
    return -1;
  }

  return listen_sd;
}

int socket_setup( int listen_sd )
{
  if ( 0 > listen_sd )  { return -1; }

  int on = 1;

  /* Allow socket descriptor to be reuseable                   */
  if ( 0 > setsockopt( listen_sd, SOL_SOCKET,
            SO_REUSEADDR, (char *)&on, sizeof(on)) )
  {
    perror("setsockopt() failed");
    close(listen_sd);
    return -1;
  }

  /* Set socket to be nonblocking. All of the sockets for      */
  /* the incoming connections will also be nonblocking since   */
  /* they will inherit that state from the listening socket.   */
  if ( 0 > socket_set_nonblocking( listen_sd ) )
  {
    perror("ioctl() failed");
    close(listen_sd);
    return -1;
  }

  return 1;
}

int socket_bind( int listen_sd, char *host, int port )
{
  if ( 0 > listen_sd )  { return -1; }

  struct sockaddr_in6 addr;
  memset( &addr, 0, sizeof(addr) );
  addr.sin6_family      = AF_INET6;
  memcpy( &addr.sin6_addr, &in6addr_any, sizeof(in6addr_any) );
  addr.sin6_port        = htons( port );

  if ( 0 > bind(listen_sd, (struct sockaddr *)&addr, sizeof(addr) ) )
  {
    perror("bind() failed");
    close(listen_sd);
    return -1;
  }

  return 1;
}

int socket_listen(int listen_sd, int backlog)
{
  if ( 0 > listen_sd || 0 > backlog )  { return -1; }

  if ( 0 > listen(listen_sd, backlog) )
  {
    perror("listen() failed");
    close(listen_sd);
    return -1;
  }

  return 1;
}

// Thank you internet for this wonderful information
// I didn't know that you can do the #if and #else
int socket_set_nonblocking( int fd )
{
    int flags;

    /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
    /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    /* Otherwise, use the old way of doing it */
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
}
