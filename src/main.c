
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

  int x = 169;
  float z = square_root( x );
  printf( "sqrt(%d) = %f\n ", x, z );
}
