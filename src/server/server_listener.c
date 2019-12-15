/**
 * This is where we will make the logic for the listener socket
 * In the future I want to use this method in a thread
 *
 * Basically accept all of the connections
 * Create an epoll event for that connection
 * Stick every one of them into a temporary buffer
 *
 * Eventually that array will be combined with the huge working array
 * If there are any new connections to add, we want to flip a flag
**/
int server_listener( int listen_sd, int epoll_fd, struct epoll_event *events, int *nfds )  {
  if ( listen_sd < 1 || epoll_fd < 1 || !events || !nfds ) { return 1; }

  int new_sd = 0;

  do {
    /* Accept each incoming connection. If               */
    /* accept fails with EWOULDBLOCK, then we            */
    /* have accepted all of them. Any other              */
    /* failure on accept will cause us to end the        */
    /* server.                                           */
    new_sd = accept(listen_sd, NULL, NULL);
    if (new_sd < 0)  {
      if (errno != EWOULDBLOCK)  {
        perror("  accept() failed");
        return 1;
      }
      return 0;
    }

    printf("  New incoming connection - %d\n", new_sd);
    events[*nfds] = epoll_add( epoll_fd, POLLIN | POLLOUT, new_sd );

    (*nfds)++; // at this point we want to document our new file descriptor
  } while (new_sd != -1);

  return 0;
}
