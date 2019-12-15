
int listening_socket_logic( int listen_sd, int epoll_fd, struct epoll_events *events, int *nfds )
{

  do
  {
    /* Accept each incoming connection. If               */
    /* accept fails with EWOULDBLOCK, then we            */
    /* have accepted all of them. Any other              */
    /* failure on accept will cause us to end the        */
    /* server.                                           */
    int new_sd = accept(listen_sd, NULL, NULL);
    if (new_sd < 0)  {
      if (errno != EWOULDBLOCK)  {
        perror("  accept() failed");
        return 1;
      }
      return 0;
    }

    /* Add the new incoming connection to the            */
    /* pollfd structure                                  */
    printf("  New incoming connection - %d\n", new_sd);
    events[*nfds] = epoll_add( epoll_fd, POLLIN | POLLOUT, new_sd );
    (*nfds)++;

    /* Loop back up and accept another incoming          */
    /* connection                                        */
  } while (new_sd != -1);

}

int connection_read( struct epoll_events *events, int i )  {

  int total, len, rc = 0;
  char buffer[80];
  printf("  Descriptor %d is readable\n", events[i].data.fd);
  /* Receive all incoming data on this socket            */
  /* before we loop back and call poll again.            */
  do
  {
    /* Here we will check for now if we need to           */
    /* stop reading from the socket or not. For           */
    /* now we'll do it just by cutting off at a           */
    /* certain number of bits                             */
    if ( total > 15 )  {
      return 0;
      // hopefully by breaking, it will not prevent
      // us from writing to the socket.
    }

    /* Receive data on this connection until the         */
    /* recv fails with EWOULDBLOCK. If any other         */
    /* failure occurs, we will close the                 */
    /* connection.                                       */
    rc = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
    if (rc < 0)  {
      if (errno != EWOULDBLOCK) {
        perror("  recv() failed");
        return 1;
      }
      return 0;
    }

    /* Check to see if the connection has been           */
    /* closed by the client                              */
    if (rc == 0)
    {
      printf("  Connection closed\n");
      return 1;
    }

    /* Data was received                                 */
    len = rc;
    total += len;
    printf("  %d bytes received\n", len);

  } while(TRUE);
}

int connection_write( struct epoll_events *events, int i )  {

  char *msg = "HTTP/1.1 200 OK\r\nDate: Mon, 27 Jul 2009 12:28:53 GMT\r\nServer: Apache/2.2.14 (Win32)\r\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\nContent-Length: 51\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\n<h1>What it do baby</h1><h4>Sara b is a baddie</h4>";
  int len = strlen(msg);
  int rc = send(events[i].data.fd, msg, len, 0);

  if (rc < 0) { perror("  send() failed"); }
  else { printf( "%d bytes sent in response\n", len ); }

}

int clean_up(int epoll_fd, struct epoll_event *events, int nfds)  {

  for (int i = 0; i < nfds; i++)
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
