// Copyright (c) 2019 HG King Development and Design All Rights Reserved.
// mem.h file

#ifndef MAJIN_MEM_H
#define MAJIN_MEM_H

typedef enum { FALSE, TRUE } bool;

void* copy(void*dst0,const void*src0,const size_t len);
int compare( const void*x,const void*y,const size_t len);

#endif
