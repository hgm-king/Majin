// Copyright (c) 2019 HG King Development and Design All Rights Reserved.
// sstring.h file

#ifndef MAJIN_STRINGS_H
#define MAJIN_STRINGS_H


bool string_valid(const char*str, const size_t len);
bool string_duplicate( char*dst,const char*src,const size_t len);
bool string_equal( const char*x,const char*y,const size_t len);
int string_length(const char*str,const size_t len);
int string_element(const char needle,const char*haystack, const size_t len);
int string_tokenize(const char*str,const char*delims,const size_t str_len,const size_t delims_len,char**tok,const size_t max_tok_len,const size_t req_tok);
bool string_to_int(const char*s,int*v);
bool string_repetition(char*dst,const char*src,const size_t len,const size_t num);
bool string_concatenate(char*dst,const char*a,const size_t a_l,const char*b,const size_t b_l);
char* string_generate(const char*src,size_t*len);


#endif
