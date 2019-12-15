// Copyright (c) 2019 HG King Development and Design All Rights Reserved.
// epoll.c file

#include <sys/epoll.h>
#include "epoll.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct epoll_event epoll_add( int epoll_fd, int events, int fd )
{
  struct epoll_event e = {};

  if ( 0 > epoll_fd || 0 > events || 0 > fd )  { return e; }

  e.events = events;
  e.data.fd = fd;
  if ( !epoll_ctl( epoll_fd, EPOLL_CTL_ADD, fd, &e ) )  {
    return e;
  }

  return e;
}

int compress_poll_array ( struct epoll_event *events, size_t nfds )
{
  if ( !events && !nfds )  { return 0; }
  int n = compact( events, nfds, 0, nfds - 1 );
  printf( "COMPACTING :: %zu -> %d\n", nfds, n );
  return n;
}

int compact( struct epoll_event *events, size_t n, int l, int r )
{
	if (
	  !events ||
	  l < 0 ||
	  l > n ||
	  r < 0 ||
	  r > n
	)  { return l; }

	while ( events[l].data.fd != -1 )  { l++; }
	while ( events[r].data.fd == -1 )  { r--; }

	if ( l <= r )  {
		swap( events, l, r );
		l = compact( events, n, l, r );
	}

  return l;
}

void swap ( struct epoll_event *array, int i, int j )
{
	if ( !array || i < 0 || j < 0 )  { return; }

	int hold = array[i].data.fd;
	array[i].data.fd = array[j].data.fd;
	array[j].data.fd = hold;
}
