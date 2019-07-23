// Copyright (c) 2019 HG King Development and Design All Rights Reserved.
// mem.c file

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"

void* copy( void* dst0, const void* src0, const size_t len )
{
	if ( !dst0 || !src0 || src0 == dst0 || 0 == len )  { return NULL; }
	char* dst = (char*)dst0;
	char* src = (char*)src0;
	for ( size_t i = 0; i < len; i++ )  { dst[i] = src[i]; }
	return (void*)dst;
}

int compare( const void* x, const void* y, const size_t len )
{
	if ( x && y && 0 != len )  {
		char* a = (char*)x;
		char* b = (char*)y;
		for ( size_t i = 0; i < len; i++ )  {
			int dif = a[i] - b[i];
			if ( 0 != dif )  { return dif; }
		}
	}
	return 0;
}
