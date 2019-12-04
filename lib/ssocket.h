// Copyright (c) 2019 HG King Development and Design All Rights Reserved.
// ssocket.h file

#ifndef MAJIN_SOCKET_H
#define MAJIN_SOCKET_H


int socket_create( char *host, int port, int backlog );
int socket_setup( int listen_sd );
int socket_bind( int listen_sd, char *host, int port );
int socket_listen(int listen_sd, int backlog);
int socket_set_nonblocking(int fd);


#endif
