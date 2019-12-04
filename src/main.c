#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include "mem.h"
#include "sstring.h"
#include "llist.h"
#include "dynarray.h"
#include "mmath.h"
#include "tree.h"
#include "ssocket.h"

#define SERVER_PORT     6950
#define BACKLOG           64
#define HOST            NULL
#define MAX_EVENTS       200

int majin_compress_poll_array ( struct epoll_event *events, size_t nfds );
struct epoll_event epoll_add( int epoll_fd, int events, int fd );

int main ( int argc, char** argv )
{
  int    len, rc;
  int    listen_sd = -1, new_sd = -1;
  int    end_server = FALSE, compress_array = FALSE;
  int    close_conn;
  char   buffer[80];
  int    timeout;
  struct epoll_event events[MAX_EVENTS];
  int    total[200];
  int    nfds = 1, current_size = 0, i;

  /* Create an AF_INET6 stream socket to receive incoming      */
  /* connections on                                            */
  listen_sd = socket_create(NULL, SERVER_PORT, BACKLOG);
  if (0 > listen_sd )
  {
    perror("socket() failed");
    exit(-1);
  }

  /* Initialize the pollfd structure                           */
  memset(events, 0 , sizeof(events));

  int epoll_fd = epoll_create1(0);
  if(epoll_fd == -1)
  {
    fprintf(stderr, "Failed to create epoll file descriptor\n");
    return 1;
  }

  events[0] = epoll_add( epoll_fd, EPOLLIN, listen_sd );

  /* Initialize the timeout to 3 minutes. If no                */
  /* activity after 3 minutes this program will end.           */
  /* timeout value is based on milliseconds.                   */
  timeout = (3 * 60 * 1000);

  /* Loop waiting for incoming connects or for incoming data   */
  /* on any of the connected sockets.                          */
  do
  {
    /* Call poll() and wait 3 minutes for it to complete.      */
    printf("Waiting on poll()...\n");
    nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, timeout);

    /* One or more descriptors are readable.  Need to          */
    /* determine which ones they are.                          */
    current_size = nfds;
    for (i = 0; i < current_size; i++)
    {
      /* Loop through to find the descriptors that returned    */
      /* POLLIN and determine whether it's the listening       */
      /* or the active connection.                             */
      if(events[i].events == 0)
        continue;

      /* If revents is not POLLIN, it's an unexpected result,  */
      /* log and end the server.                               */
      if( !(events[i].events & EPOLLIN) && !(events[i].events & EPOLLOUT) )
      {
        printf("  Error! revents = %d\n", events[i].events);
        end_server = TRUE;
        break;
      }

      if ( events[i].data.fd == listen_sd )
      {
        /* Listening descriptor is readable.                   */
        printf("  Listening socket is readable\n");

        /* Accept all incoming connections that are            */
        /* queued up on the listening socket before we         */
        /* loop back and call poll again.                      */
        do
        {
          /* Accept each incoming connection. If               */
          /* accept fails with EWOULDBLOCK, then we            */
          /* have accepted all of them. Any other              */
          /* failure on accept will cause us to end the        */
          /* server.                                           */
          new_sd = accept(listen_sd, NULL, NULL);
          if (new_sd < 0)
          {
            if (errno != EWOULDBLOCK)
            {
              perror("  accept() failed");
              end_server = TRUE;
            }
            break;
          }

          /* Add the new incoming connection to the            */
          /* pollfd structure                                  */
          printf("  New incoming connection - %d\n", new_sd);
          events[nfds] = epoll_add( epoll_fd, POLLIN | POLLOUT, new_sd );
          nfds++;

          /* Loop back up and accept another incoming          */
          /* connection                                        */
        } while (new_sd != -1);
      }

      /* This is not the listening socket, therefore an        */
      /* existing connection must be readable                  */

      else
      {
        printf("  Descriptor %d is ready\n", events[i].data.fd);
        close_conn = FALSE;

        if ( events[i].events & EPOLLIN )
        {
          printf("  Descriptor %d is readable\n", events[i].data.fd);
          /* Receive all incoming data on this socket            */
          /* before we loop back and call poll again.            */
          do
          {
            /* Here we will check for now if we need to           */
            /* stop reading from the socket or not. For           */
            /* now we'll do it just by cutting off at a           */
            /* certain number of bits                             */
            if ( total[i] > 15 )  {
              break;
              // hopefully by breaking, it will not prevent
              // us from writing to the socket.
            }

            /* Receive data on this connection until the         */
            /* recv fails with EWOULDBLOCK. If any other         */
            /* failure occurs, we will close the                 */
            /* connection.                                       */
            rc = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
            if (rc < 0)
            {
              if (errno != EWOULDBLOCK)
              {
                perror("  recv() failed");
                close_conn = TRUE;
              }
              break;
            }

            /* Check to see if the connection has been           */
            /* closed by the client                              */
            if (rc == 0)
            {
              printf("  Connection closed\n");
              close_conn = TRUE;
              break;
            }

            /* Data was received                                 */
            len = rc;
            total[i] += len;
            printf("  %d bytes received\n", len);

            // /* Echo the data back to the client                  */
            // rc = send(fds[i].fd, buffer, len, 0);
            // if (rc < 0)
            // {
            //   perror("  send() failed");
            //   close_conn = TRUE;
            //   break;
            // }



          } while(TRUE);

        }

        if ( events[i].events & EPOLLOUT )
        {
          printf("  Descriptor %d is writable\n", events[i].data.fd);
          char *msg = "HTTP/1.1 200 OK\r\nDate: Mon, 27 Jul 2009 12:28:53 GMT\r\nServer: Apache/2.2.14 (Win32)\r\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\nContent-Length: 51\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\n<h1>What it do baby</h1><h4>Sara b is a baddie</h4>";
          len = strlen(msg);
          rc = send(events[i].data.fd, msg, len, 0);
          if (rc < 0)
          {
            perror("  send() failed");
          }
          else
          {
            printf( "%d bytes sent in response\n", len );
          }
          close_conn = TRUE;
        }
        /* If the close_conn flag was turned on, we need       */
        /* to clean up this active connection. This            */
        /* clean up process includes removing the              */
        /* descriptor.                                         */
        if (close_conn)
        {
          close(events[i].data.fd);
          events[i].data.fd = -1;
          total[i] = 0;
          compress_array = TRUE;
        }


      }  /* End of existing connection is readable             */
    } /* End of loop through pollable descriptors              */

    /* If the compress_array flag was turned on, we need       */
    /* to squeeze together the array and decrement the number  */
    /* of file descriptors. We do not need to move back the    */
    /* events and revents fields because the events will always*/
    /* be POLLIN in this case, and revents is output.          */
    if (compress_array)
    {
      compress_array = FALSE;
      nfds = majin_compress_poll_array( events, nfds );
    }

  } while (end_server == FALSE); /* End of serving running.    */

  /* Clean up all of the sockets that are open                 */
  for (i = 0; i < nfds; i++)
  {
    if(events[i].data.fd >= 0)
      close(events[i].data.fd);
  }

  if(close(epoll_fd))
  {
    fprintf(stderr, "Failed to close epoll file descriptor\n");
    return 1;
  }
}



struct epoll_event epoll_add( int epoll_fd, int events, int fd )
{
  struct epoll_event e;

  e.events = events;
  e.data.fd = fd;
  if ( !epoll_ctl( epoll_fd, EPOLL_CTL_ADD, fd, &e ) )  {
    return e;
  }

  return e;
}

int majin_compress_poll_array ( struct epoll_event *events, size_t nfds )
{
  if ( !events && !nfds )  { return 0; }

  for ( size_t i = 0; i < nfds; i++ )
  {
    if (events[i].data.fd == -1)
    {
      for( size_t j = i; j < nfds; j++ )
      {
        events[j].data.fd = events[j+1].data.fd;
      }
      i--;
      nfds--;
    }
  }

  return nfds;
}
