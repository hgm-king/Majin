// Copyright (c) 2019 HG King Development and Design All Rights Reserved.
// llist.c file

#include <stdio.h>
#include <stdlib.h>
#include "mmath.h"

float power( float b, int p )
{
  // b ^  0 = 1
  // 0 ^  p = 0
  // b ^ -p = 1 / (b ^ p)
  // b ^  p = (b ^ p) ^ 2 where p is even
  // b ^  p = b * (b ^ p) ^ 2 where p is odd

  if ( b == 0 )  { return 0; }
  if ( p == 0 )  { return 1; }
  float n = 0;
  if ( p < 0 )  { n = power( 1 / b , ( -p ) / 2 ); }
  else { n = power( b , p / 2 ); }

  printf( "b:%.2f p:%d n:%.2f\n", b, p, n );

  if ( 0 == p % 2 )  { return n * n; }
  return b * n * n;
}

float power_prime( float b, float p )
{
  return p * power( b , p - 1 );
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
