
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

  for ( int i = -5; i < 15; i++ )
  {
    for ( int j = -5; j < 15; j++ )
    {
      int z = floor(power( i, ++j ) + 0.5 );
      int zed = pow( i, j );
      int dif = z - zed;

      if ( !dif )  { printf( ":)\n" ); }
      else  { printf( "%d^%d = %d, %d\n", i, j, z, dif ); }
    }
  }
}
