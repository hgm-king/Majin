#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netdb.h>
#include <poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define MYPORT "6950"  // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold


size_t writenw(int fd, char *buf, size_t n);
void poll_wait(int fd, int events);
size_t readmore(int sock, char *buf, size_t n);
size_t writenw(int fd, char *buf, size_t n);
size_t readall(int sock, char *buf, size_t n);

int prime_addrinfo( struct addrinfo *hints );


int main(int argc, char *argv[])
{

  struct sockaddr_storage their_addr;
  int status;
  struct addrinfo hints;
  struct addrinfo *res;  // will point to the results


  memset( &hints, 0, sizeof hints );  // make sure the struct is empty
  hints.ai_family = AF_UNSPEC;        // don't care IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;    // TCP stream sockets
  hints.ai_flags = AI_PASSIVE;        // fill in my IP for me

  if ( ( status = getaddrinfo(NULL, MYPORT, &hints, &res ) ) != 0 )  {
    fprintf( stderr, "getaddrinfo error: %s\n", gai_strerror( status ) );
    exit( 1 );
  }

  // servinfo now points to a linked list of 1 or more struct addrinfos
  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if ( sockfd == -1 ) {
    perror("socket"); // or use strerror()
    exit( 1 );
  }

  bind(sockfd, res->ai_addr, res->ai_addrlen);
  fprintf( stdout, "Bound!\n" );

  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen"); // or use strerror()
  } else {
    fprintf( stdout, "should be listening\n" );
  }

  // now accept an incoming connection:
  char ipstr[INET6_ADDRSTRLEN];
  socklen_t addr_size = sizeof their_addr;
  int i = 0;

  while ( 1 )
  {

    int new_fd = accept( sockfd, (struct sockaddr *)&their_addr, &addr_size );

    /* set socket non blocking */
    int fl = fcntl(new_fd, F_GETFL);
    fcntl(new_fd, F_SETFL, fl | O_NONBLOCK);

    char *msg = "HTTP/1.1 200 OK\r\nContent-type:text/html\r\n\r\n<h1>What it do baby</h1><h4>Sara b is a baddie</h4>";
    int len, bytes_sent;
    fprintf( stdout, "Okay we got a connection\n" );
    len = strlen(msg);
    bytes_sent = writenw( new_fd, msg, len);
    //fprintf( stdout, "sent %d bytes\n", bytes_sent );

    len = strlen(ipstr);
    bytes_sent += writenw( new_fd, ipstr, len);
    fprintf( stdout, "%d connections\n", i++ );
    close(new_fd);

  }
  close(sockfd);
  freeaddrinfo(res); // free the linked-list

  return 0;
}

void poll_wait(int fd, int events)
{
    int n;
    struct pollfd pollfds[1];
    memset((char *) &pollfds, 0, sizeof(pollfds));

    pollfds[0].fd = fd;
    pollfds[0].events = events;

    n = poll(pollfds, 1, -1);
    if (n < 0) {
        perror("poll()");
        exit(1);
    }
}

/*
This is here as a sort of config.
Really it is an easily separated block of code,
so I got it out of the way.
*/
int prime_addrinfo( struct addrinfo *hints )
{
  if ( !hints )  { return 0; }

  memset( &hints, 0, sizeof hints );  // make sure the struct is empty
  hints->ai_family = AF_UNSPEC;        // don't care IPv4 or IPv6
  hints->ai_socktype = SOCK_STREAM;    // TCP stream sockets
  hints->ai_flags = AI_PASSIVE;        // fill in my IP for me

  return 1;
}

size_t readmore(int sock, char *buf, size_t n) {

        fd_set rfds;
        int ret, bytes;



        poll_wait(sock,POLLERR | POLLIN );
        bytes = readall(sock, buf, n);

        if (0 == bytes) {
                perror("Connection closed");
                exit(1);
                /* NOT REACHED */
        }

        return bytes;
}

size_t readall(int sock, char *buf, size_t n) {
        size_t pos = 0;
        ssize_t res;

        while (n > pos) {
                res = read (sock, buf + pos, n - pos);
                switch ((int)res) {
                        case -1:
                                if (errno == EINTR || errno == EAGAIN)
                                        continue;
                                return 0;
                        case 0:
                                errno = EPIPE;
                                return pos;
                        default:
                                pos += (size_t)res;
                }
        }
        return (pos);
}


size_t writenw(int fd, char *buf, size_t n)
{
        size_t pos = 0;
        ssize_t res;
        while (n > pos) {
                poll_wait(fd, POLLOUT | POLLERR);
                res = write (fd, buf + pos, n - pos);
                switch ((int)res) {
                        case -1:
                                if (errno == EINTR || errno == EAGAIN)
                                        continue;
                                return 0;
                        case 0:
                                errno = EPIPE;
                                return pos;
                        default:
                                pos += (size_t)res;
                }
        }
        return (pos);

}
