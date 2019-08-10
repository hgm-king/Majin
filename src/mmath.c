// Copyright (c) 2019 HG King Development and Design All Rights Reserved.
// llist.c file

#include <stdio.h>
#include <stdlib.h>
#include "mmath.h"

float power( float b, int p )
{
  if ( 0 == p )  { return 1; }

  float n = power( b , p / 2 );
  if ( 0 == p % 2 )  { return n * n; }
  if ( p > 0 )  { return b * n * n; }
  return ( n * n ) / b;
}

float square_root( float x )
{
  float z = x;
	float z_last = 1;
	for ( int i = 0; i < 10; i++ )  {

    float exp = power( z, 2 );
		z -= (exp - x) / ( 2 *z );

		float dif = absolute_value(z_last - z);
		if(dif < 0.00000001) return z;

		z_last = z;
	}
  return 0;
}

int div()
{
  // Express D as M × 2e where 1 ≤ M < 2 (standard floating point representation)
  // D' := D / 2e+1   // scale between 0.5 and 1, can be performed with bit shift / exponent subtraction
  // N' := N / 2e+1
  // X := 48/17 − 32/17 × D'   // precompute constants with same precision as D
  // repeat p times   // can be precomputed based on fixed P
  //     X := X + X × (1 - D' × X)
  // end
  // return N' × X
}

float absolute_value( float x )
{
  if ( x < 0 ) return -x;
  return x;
}

int bottom( float x )
{
  return x;
}

int top( float x )
{
  return x;
}
