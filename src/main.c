#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "mem.h"
#include "sstring.h"
#include "llist.h"
#include "dynarray.h"
#include "mmath.h"
#include "tree.h"

void dub(void *element, size_t size);

int main( void )
{
  srand(time(0));

  printf( "HELLO WORLD\n\n" );

  tree_node_t *head = tree_node_create( 20 );
  if ( !head )  {
    fprintf( stderr, "No head created\n" );
    exit(1);
  } else {
    printf( "Leggooo\n" );
  }

  size_t x = 50;
  size_t y = 50;

  for(int i = 0; i<5; i++)
        printf(" %d ", rand());

  for ( size_t i = 1; i < x; i++ )
  {
    for ( size_t j = 1; j < y; j++ )
    {
      int x = rand() % (i * j);
      printf( "%d\n", x );
      tree_insert( head, x );
    }
  }

  tree_inorder_walk( head );

  tree_destroy( head );



/*
  dynarray_t *darr = dynarray_generate();
  size_t len = 50;

  if ( !dynarray_initialize( darr, sizeof( int ), len + 1 ) )  {
    fprintf( stderr, "Could not initialize darr\n" );
    exit( 1 );
  }

  printf( "The stats for our array s::%zu c::%zu\n", darr->size, darr->count );

  for ( size_t i = 0; i < len; i++ )
  {
    int x = i;
    printf( "*** %d ***\n", x );
    dynarray_write( darr, &x, i, 1 );
  }

  dynarray_iterate( darr, dub );

  int buff;
  for ( size_t i = 0; i < len; i++ )
  {
    dynarray_read( darr, &buff, i, 1 );
    printf( "%zu :: %d\n", i, buff );
  }

  dynarray_destroy( darr );
*/
}

void dub(void *element, size_t size)
{
  if ( !element || !size )  { return; }
  // hmm I want to make the pointer casting automatic
  int *val = (int*)element;
  *val *= 2;
  // but this is pretty darn sweet
  element = (void*)val;
}
