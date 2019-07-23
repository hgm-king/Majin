// Copyright (c) 2019 HG King Development and Design All Rights Reserved.
// dynarray.h file

#ifndef MAJIN_DYNARRAY_H
#define MAJIN_DYNARRAY_H


typedef struct dynarray {
  size_t size; // how big
  size_t count; // how many
  size_t used;
  void *data; // pointer to data aka address
} dynarray_t;

void* memcpy(void* dst_, const void* src_, const size_t len);
dynarray_t *dynarray_generate(void);
bool dynarray_initialize(dynarray_t *darr, size_t size, size_t count);
bool dynarray_grow(dynarray_t *darr, size_t needed);
bool dynarray_destroy(dynarray_t *darr);
bool dynarray_read(dynarray_t *darr, void *dst, size_t index, size_t count);
bool dynarray_write(dynarray_t *darr, void *src, size_t index, size_t count);


#endif
