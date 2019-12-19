#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "sstring.h"
#include "llist.h"
#include "dynarray.h"
#include "mmath.h"
#include "tree.h"
#include "server.h"

#define SERVER_PORT     6950
#define BACKLOG          64
#define HOST            NULL
#define MAX_EVENTS       200


int main ( int argc, char** argv )
{
  fprintf( stdout, "Hello World!!\n\n" );

  server_t *server = server_generate();
  if ( !server_initialize( server, SERVER_PORT, HOST, BACKLOG, MAX_EVENTS ) )  {
    fprintf( stdout, "Error: we had an issue creating the server\n\n" );
    exit(1);
  }

  server_run( server );

  if( !server_destroy( server ) )  {
    fprintf( stdout, "Error: we had an issue destroying the server\n\n" );
    exit(1);
  }
}
