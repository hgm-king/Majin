// Copyright (c) 2019 HG King Development and Design All Rights Reserved.
// dynarray.h file

#ifndef MAJIN_SERVER_H
#define MAJIN_SERVER_H

typedef struct server {
  struct epoll_events *events;
  size_t events_length;
  size_t events_used;
  int listen_sd;
  int state;
  int timeout;
  struct epoll_events *events_buffer;
  size_t events_buffer_length;
  size_t events_buffer_used;
  int port;
  char *host;
  int backlog;
  int max_events;
} server_t;


server_t server_generate( void );
int server_initialize( server_t *server, int port, char *host, int backlog, int max_events );
int server_destroy( server_t *server );
int server_run( server_t *server );


#endif
