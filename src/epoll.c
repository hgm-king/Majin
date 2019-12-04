#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_EVENTS 5
#define READ_SIZE 10

int epoll_add( int epoll_fd, int events, int fd );

int main ( void )
{

  int running = 1, event_count, i;
  size_t bytes_read;
  char read_buffer[READ_SIZE + 1];
  struct epoll_event event, events[MAX_EVENTS];
  int epoll_fd = epoll_create1(0);

  if(epoll_fd == -1)
  {
    fprintf(stderr, "Failed to create epoll file descriptor\n");
    return 1;
  }


  if( 0 > epoll_add( epoll_fd, EPOLLIN, 0 ) )
  {
    fprintf(stderr, "Failed to add file descriptor to epoll\n");
    close(epoll_fd);
    return 1;
  }

  while(running)
  {
    printf("\nPolling for input...\n");
    event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000);
    printf("%d ready events\n", event_count);
    for(i = 0; i < event_count; i++)
    {
      printf("Reading file descriptor '%d' -- ", events[i].data.fd);
      bytes_read = read(events[i].data.fd, read_buffer, READ_SIZE);
      printf("%zd bytes read.\n", bytes_read);
      read_buffer[bytes_read] = '\0';
      printf("Read '%s'\n", read_buffer);

      if(!strncmp(read_buffer, "stop\n", 5))
        running = 0;
    }
  }


  if(close(epoll_fd))
  {
    fprintf(stderr, "Failed to close epoll file descriptor\n");
    return 1;
  }
  return 0;

}

int epoll_add( int epoll_fd, int events, int fd )
{
  if ( 0 > epoll_fd || 0 > events || 0 > fd )  { return -1; }

  struct epoll_event e;

  e.events = events;
  e.data.fd = fd;
  if ( !epoll_ctl( epoll_fd, EPOLL_CTL_ADD, fd, &e ) )  {
    return -1;
  }

  return e;
}
