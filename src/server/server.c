#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "server.h"
#include "ssocket.h"
#include "epoll.h"

/**
 * The server is going to be a struct that holds a couple of things in
 * memory
 *  - epoll_event array
 *  - length of the array
 *  - current size of that array
 *  - listen_sd ( copy of 0th elem in above array )
 *  - state
 *  - timeout
 *  - epoll_event buffer
 *
 * When we initialize, we basically need to spin up the mem for everything above
 * and set everything that is not a pointer to 0 or NUL or something like that
**/
server_t *server_generate( void )
{
  server_t *server = malloc( sizeof( server_t ) );
  if ( !server )  { return NULL; }

  server->events = NULL;
  server->events_buffer = NULL;
  server->events_length = server->events_used = 0;
  server->events_buffer_length = server->events_buffer_used = 0;
  server->epoll_fd = server->listen_sd = server->timeout = server->state = 0;
  server->port = server->backlog = server->max_events = 0;
  server->host = NULL;

  return server;
}

int server_destroy( server_t *server )
{
  if ( !server )  { return 0; }

  for (int i = 0; i < server->events_used; i++)
  {
    if((server->events)[i].data.fd >= 0)
      close((server->events)[i].data.fd);
  }

  close(server->epoll_fd);
  free( server->events );
  free( server->events_buffer );
  free( server );

  return 1;
}
/**
 * to set things up, we need to take in all of the external information that we
 * need for this server to run.
 *  - port
 *  - host
 *  - backlog
 *  - max events
 * essentially this is our configuration
 *
 * we want to create the listening socket and put it into an epoll event
**/
int server_initialize( server_t *server, int port, char *host, int backlog, int max_events )
{
  if ( !server || 0 > port || 0 > backlog || 0 > max_events )  { return 0; }

  // Step 1: initialize those paramaters into our server struct
  server->port = port;
  server->host = host;
  server->backlog = backlog;
  server->max_events = max_events;
  server->timeout = 300000;

  // Step 2: create a listening socket that will accept the connections
  server->listen_sd = socket_create(host, port, backlog);
  if ( 0 > server->listen_sd )  { return 0; }

  // Step 3: alloc and clear the array of events that we will use
  server->events = calloc( 0, sizeof( server->events ) * max_events );
  if( !server->events )  { return 0; }

  // Step 4: initialize the epoll file descriptor
  server->epoll_fd = epoll_create1(0);
  if ( -1 == server->epoll_fd )  { return 0; }

  // Step 5: create the listening socket event and set it to the first element
  server->events[0] = epoll_add( server->epoll_fd, EPOLLIN, server->listen_sd );

  // Step 6: return that AOK
  return 1;
}

/**
 * running is where all of the serious logic comes into play
 * PSUEDOCODE:
 * run()
 *   DO WHILE state != stop
 *   | poll()
 *
 *   | FOR EACH fd
 *   | | IF listen_sd
 *   | | | state = server_listener()
 *
 *   | | ELSE
 *   | | | IF readable
 *   | | | | conn_state = server_reader()
 *   | | | IF writable
 *   | | | | conn_state = server_writer()
 *
 *   | | | IF conn_state == close
 *   | | | | close_conn()
 *
 *   | IF compress_array
 *   | | compress_array()
 *   | IF expand_array
 *   | | expand_array()
 *
 *   clean_up()
**/
int server_run( server_t *server )
{
  if ( !server )  { return 0; }

  int current_size = 0;
  int i = 0;
  int end_server = 0;
  int conn_state = 0;
  int compress_array = 0;

  do
  {
    /* Call poll() and wait 3 minutes for it to complete.      */
    //printf("Waiting on poll()...\n");
    server->events_used = epoll_wait(server->epoll_fd,
                      server->events,
                      server->max_events,
                      server->timeout);

    /* One or more descriptors are readable.  Need to          */
    /* determine which ones they are.                          */
    current_size = server->events_used;
    for (i = 0; i < current_size; i++)  {
      if(server->events[i].events == 0)
        continue;

      /* If revents is not POLLIN, it's an unexpected result,  */
      /* log and end the server.                               */
      if( !(server->events[i].events & EPOLLIN) &&
          !(server->events[i].events & EPOLLOUT) )  {
        printf("  Error! revents = %d\n", server->events[i].events);
        end_server = 1;
        break;
      }

      if ( server->events[i].data.fd == server->listen_sd )  {
        /* Listening descriptor is readable.                   */
        conn_state = server_listener( server->listen_sd,
                                             server->epoll_fd,
                                             server->events,
                                             &(server->events_used) );
      }

      /* This is not the listening socket, therefore an        */
      /* existing connection must be readable                  */
      else  {
        //printf("  Descriptor %d is ready\n", server->events[i].data.fd);
        conn_state = 0;

        if ( server->events[i].events & EPOLLIN ) {
          conn_state = server_read( server->events, i );
        }

        if ( server->events[i].events & EPOLLOUT && conn_state > 0 )  {
          conn_state = server_write( server->events, i );
        }

        if ( conn_state < 0 )  {
          close(server->events[i].data.fd);
          server->events[i].data.fd = -1;
          compress_array = 1;
        }

      }  /* End of existing connection is readable             */
    } /* End of loop through pollable descriptors              */

    // Clean up here
    if (compress_array)
    {
      compress_array = 0;
      server->events_used = compress_poll_array( server->events, server->events_used );
    }

  } while (end_server == 0); /* End of serving running.    */

  return 1;
}
