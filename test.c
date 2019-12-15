#include <stdio.h>
#include <stdlib.h>

#define LEN 17

void swap ( int *array, int i, int j );
int compact ( int *array, int n, int i, int j );

int main ( void )
{
	fprintf( stdout, "Hello World!!\n\n" );

	int array[LEN] = { 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0 };

	for( int i = 0; i < LEN; i++ )  {
                fprintf( stdout, "%d,", array[i] );
        }
	fprintf( stdout, "\n" );

	int l = compact( array, LEN, 0, LEN - 1 );
	fprintf( stdout, "\n" );
	for( int i = 0; i < LEN; i++ )  {
		fprintf( stdout, "%d,", array[i] );
	}
	fprintf( stdout, "\n" );

	fprintf( stdout, "l :: %d\n", l );

}

int compact ( int *array, int n, int i, int j )
{
	if (
	  !array ||
	  i < 0 ||
	  i > n ||
	  j < 0 ||
	  j > n
	)  { return i; }

	while ( array[i] != 0 )  { i++; }
	while ( array[j] == 0 )  { j--; }

	if ( i <= j )  {
		swap( array, i, j );
		fprintf( stdout, "l is %d\n", i );
		i = compact( array, n, i, j );
	}

	return i;
}

void swap ( int *array, int i, int j )
{
	if ( !array || i < 0 || j < 0 )  { return; }

	int hold = array[i];
	array[i] = array[j];
	array[j] = hold;

	return;
}
