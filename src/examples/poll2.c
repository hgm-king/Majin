#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT   6950

#define TRUE             1
#define FALSE            0

int majin_setup_socket ( int listen_sd );
int majin_compress_poll_array ( struct pollfd *fds, size_t nfds );

int main (int argc, char *argv[])
{
  int    len, rc;
  int    listen_sd = -1, new_sd = -1;
  int    desc_ready, end_server = FALSE, compress_array = FALSE;
  int    close_conn;
  char   buffer[80];
  struct sockaddr_in6   addr;
  int    timeout;
  struct pollfd fds[200];
  int    total[200];
  int    nfds = 1, current_size = 0, i, j;

  /* Create an AF_INET6 stream socket to receive incoming      */
  /* connections on                                            */
  listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
  if (listen_sd < 0)
  {
    perror("socket() failed");
    exit(-1);
  }

  if ( !majin_setup_socket( listen_sd ) )
  {
    perror("socket setup failed");
    exit(-1);
  }

  /* Bind the socket                                           */
  memset(&addr, 0, sizeof(addr));
  addr.sin6_family      = AF_INET6;
  memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
  addr.sin6_port        = htons(SERVER_PORT);
  rc = bind(listen_sd,
            (struct sockaddr *)&addr, sizeof(addr));
  if (rc < 0)
  {
    perror("bind() failed");
    close(listen_sd);
    exit(-1);
  }

  /* Set the listen back log                                   */
  rc = listen(listen_sd, 32);
  if (rc < 0)
  {
    perror("listen() failed");
    close(listen_sd);
    exit(-1);
  }

  /* Initialize the pollfd structure                           */
  memset(fds, 0 , sizeof(fds));

  /* Set up the initial listening socket                       */
  fds[0].fd = listen_sd;
  fds[0].events = POLLIN;
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
    rc = poll(fds, nfds, timeout);

    /* Check to see if the poll call failed.                   */
    if (rc < 0)
    {
      perror("  poll() failed");
      break;
    }

    /* Check to see if the 3 minute time out expired.          */
    if (rc == 0)
    {
      printf("  poll() timed out.  End program.\n");
      break;
    }


    /* One or more descriptors are readable.  Need to          */
    /* determine which ones they are.                          */
    current_size = nfds;
    for (i = 0; i < current_size; i++)
    {
      /* Loop through to find the descriptors that returned    */
      /* POLLIN and determine whether it's the listening       */
      /* or the active connection.                             */
      if(fds[i].revents == 0)
        continue;

      /* If revents is not POLLIN, it's an unexpected result,  */
      /* log and end the server.                               */
      if( !(fds[i].revents & POLLIN) && !(fds[i].revents & POLLOUT) )
      {

        printf("  Error! revents = %d\n", fds[i].revents);
        end_server = TRUE;
        break;

      }
      if ( fds[i].fd == listen_sd )
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
          fds[nfds].fd = new_sd;
          fds[nfds].events = POLLIN | POLLOUT;
          nfds++;

          /* Loop back up and accept another incoming          */
          /* connection                                        */
        } while (new_sd != -1);
      }

      /* This is not the listening socket, therefore an        */
      /* existing connection must be readable                  */

      else
      {
        printf("  Descriptor %d is ready\n", fds[i].fd);
        close_conn = FALSE;

        if ( fds[i].revents & POLLIN )
        {
          printf("  Descriptor %d is readable\n", fds[i].fd);
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
            rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
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

        if ( fds[i].revents & POLLOUT )
        {
          printf("  Descriptor %d is writable\n", fds[i].fd);
          char *msg = "HTTP/1.1 200 OK\r\nDate: Mon, 27 Jul 2009 12:28:53 GMT\r\nServer: Apache/2.2.14 (Win32)\r\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\nContent-Length: 51\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\n<h1>What it do baby</h1><h4>Sara b is a baddie</h4>";
          len = strlen(msg);
          rc = send(fds[i].fd, msg, len, 0);
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
          close(fds[i].fd);
          fds[i].fd = -1;
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
      nfds = majin_compress_poll_array( fds, nfds );
    }

  } while (end_server == FALSE); /* End of serving running.    */

  /* Clean up all of the sockets that are open                 */
  for (i = 0; i < nfds; i++)
  {
    if(fds[i].fd >= 0)
      close(fds[i].fd);
  }
}

int majin_setup_socket ( int listen_sd )
{
  if ( 0 > listen_sd )  { return 0; }

  int rc, on = 1;

  /* Allow socket descriptor to be reuseable                   */
  rc = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR,
                  (char *)&on, sizeof(on));
  if (rc < 0)
  {
    perror("setsockopt() failed");
    close(listen_sd);
    return -1;
  }

  /* Set socket to be nonblocking. All of the sockets for      */
  /* the incoming connections will also be nonblocking since   */
  /* they will inherit that state from the listening socket.   */
  rc = ioctl(listen_sd, FIONBIO, (char *)&on);
  if (rc < 0)
  {
    perror("ioctl() failed");
    close(listen_sd);
    return -1;
  }

  return listen_sd;
}

int majin_compress_poll_array ( struct pollfd *fds, size_t nfds )
{
  if ( !fds && !nfds )  { return 0; }

  for ( size_t i = 0; i < nfds; i++ )
  {
    if (fds[i].fd == -1)
    {
      for( size_t j = i; j < nfds; j++ )
      {
        fds[j].fd = fds[j+1].fd;
      }
      i--;
      nfds--;
    }
  }
}
