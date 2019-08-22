#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>

#define MYPORT "6950"  // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold

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

  if ( ( status = getaddrinfo(NULL, "6950", &hints, &res ) ) != 0 )  {
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

    int addr = getpeername( new_fd, (struct sockaddr *)&their_addr, &addr_size );
    inet_ntop( res->ai_family, (void *)&their_addr, ipstr, sizeof ipstr );

    char *msg = "HTTP/1.1 200 OK\r\nContent-type:text/html\r\n\r\n<h1>What it do baby</h1><h4>Sara b is a baddie</h4>";
    int len, bytes_sent;
    fprintf( stdout, "Okay we got a connection\n" );
    len = strlen(msg);
    bytes_sent = send(new_fd, msg, len, 0);
    //fprintf( stdout, "sent %d bytes\n", bytes_sent );

    len = strlen(ipstr);
    bytes_sent = send(new_fd, ipstr, len, 0);
    fprintf( stdout, "%d connections\n", i++ );
    close(new_fd);
  }
  close(sockfd);
  freeaddrinfo(res); // free the linked-list

  return 0;
}
