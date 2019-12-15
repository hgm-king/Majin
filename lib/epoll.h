// Copyright (c) 2019 HG King Development and Design All Rights Reserved.
// epoll.h file

#ifndef MAJIN_EPOLL_H
#define MAJIN_EPOLL_H


struct epoll_event epoll_add( int epoll_fd, int events, int fd );
int compress_poll_array ( struct epoll_event *events, size_t nfds );
int compact( struct epoll_event *events, size_t n, int l, int r );
void swap( struct epoll_event *array, int i, int j );

#endif
