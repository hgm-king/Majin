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

int top( float x )
{
  return x;
}
