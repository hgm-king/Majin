// Copyright (c) 2019 HG King Development and Design All Rights Reserved.
// dynarray.c file

#include <stdlib.h>
#include <stdio.h>
#include "mem.h"
#include "dynarray.h"

dynarray_t *dynarray_generate(void)
{
  dynarray_t *darr = malloc( sizeof ( dynarray_t ) );
  if ( !darr )  { return NULL; }
  darr->data = NULL;
  darr->size = 0;
  darr->count = 0;
  darr->used = 0;
  return darr;
}

bool dynarray_initialize(dynarray_t *darr, size_t size, size_t count)
{
  if ( !darr || 0 == size || 0 == count )  { return FALSE; }

  darr->size = size;
  darr->count = count;
  //darr->data = calloc( count, size );
  darr->data = malloc( count * size );

  if ( !darr->data )  { return FALSE; }
  return TRUE;
}

bool dynarray_grow(dynarray_t *darr, size_t needed )
{
  if ( !darr )  { return FALSE; }

  needed <<= 2;
  darr->data = realloc( darr->data, needed * darr->size );
  if ( !darr->data )  { return FALSE; }
  darr->count = needed;
  return TRUE;
}

bool dynarray_destroy(dynarray_t *darr)
{
  if ( !darr || !darr->data )  { return FALSE; }

  darr->size = 0;
  darr->count = 0;

  free( darr->data );
  free( darr );

  return TRUE;
}

bool dynarray_read(dynarray_t *darr, void *dst, size_t index, size_t count)
{
  if ( !darr || 0 > index || 0 > count )  { return FALSE; }

  size_t length = darr->size * count;         // num bytes of src
  size_t offset = darr->size * index;         // num bytes to start write
  //size_t distance = count + index;            // elements used
  size_t capacity = darr->size * darr->count; // current available bytes
  size_t needed = offset + length;            // used + needed

	printf( "index:%zu, count:%zu\n", index, count );
  printf( "length:%zu, offset:%zu, capacity:%zu, needed:%zu\n", length, offset, capacity, needed);

  if ( needed <= capacity && !copy( dst, darr->data + offset, length ) )  { return FALSE; }
  return TRUE;
}

bool dynarray_write(dynarray_t *darr, void *src, size_t index, size_t count)
{
  if ( !darr || 0 > index || 0 > count )  { return FALSE; }

  size_t length = darr->size * count;         // num bytes of src
  size_t offset = darr->size * index;         // num bytes to start write
  size_t distance = count + index;            // elements used
  size_t capacity = darr->size * darr->count; // current available bytes
  size_t needed = offset + length;            // used + needed

  //printf( "index:%zu, count:%zu\n", index, count );
  //printf( "length:%zu, offset:%zu, capacity:%zu, needed:%zu\n", length, offset, capacity, needed);

  if ( needed >= capacity && !dynarray_grow( darr, distance ) )  { return FALSE; }

  if ( !copy( darr->data + offset, src, length ) )  { return FALSE; }
  if ( distance > darr->used )  { darr->used = distance; }

  return TRUE;
}
