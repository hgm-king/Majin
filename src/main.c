
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include "mem.h"
#include "sstring.h"
#include "llist.h"
#include "dynarray.h"
#include "mmath.h"

int main( void )
{
  printf( "HELLO WORLD\n\n" );

  int z = 0;
  for ( int i = -2; i < 10; i++ )
  {
    for ( int j = -2; j < 10; j++ )
    {
      z = power( i, j );
      printf( "%d^%d = %d => ", i, j, z );
      if ( z == pow( i, j ) )  { printf( ":)\n" ); }
      else  { printf( "FALSE\n" ); }
    }
  }
}
