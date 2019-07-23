//Copyright (c) 2018 HG King Development and Design All Rights Reserved.
// sstring.c file

#include <stdlib.h>
#include <stdio.h>
#include "mem.h"
#include "sstring.h"


bool string_valid( const char *str, const size_t len )
{
	if ( str && 0 != len )  {
		for ( size_t i = 0; i < len; i++ )  {
			if ( '\0' == str[i] )  { return TRUE; }
		}
	}
	return FALSE;
}

bool string_duplicate( char *dst, const char *src, const size_t len )
{
	if ( !dst || !src || 0 == len || !copy( dst, src, len ) )  { return FALSE; }
	return TRUE;
}

bool string_equal( const char *x, const char *y, const size_t len )
{
	if ( !x || !y || 0 == len || 0 != compare( x, y, len ) )  { return FALSE; }
	return TRUE;
}

int string_length( const char *str, const size_t len )
{
	if ( ( !str && 0 == len ) || !string_valid( str, len ) )  { return -1; }
	return string_element( '\0', str, len );
}

int string_element( const char needle, const char* haystack, const size_t len )
{
	if ( haystack && 0 != len )  {
		for ( size_t i = 0; i < len; i++ )  {
			if ( needle == haystack[i] )  { return i; }
		}
	}
	return -1;
}

int string_tokenize( const char *str, const char *delims, const size_t str_len,
	const size_t delims_len, char **tok, const size_t max_tok_len, const size_t req_tok )
{
	if (
		!str || !delims || !tok ||
		0 == str_len || 0 == max_tok_len || 0 == delims_len
	)  { return 0; }

	size_t k = 0;
	size_t j = 0;

	for ( size_t i = 0; i < str_len; i++ )  {
		if ( k > req_tok || j > max_tok_len )  { return -1; }

		if ( 0 <= string_element( str[i], delims, delims_len ) )  {
			tok[k] = string_generate( str + ( i - j ), &j );
			k++;
			j = 0;
		} else {
			j++;
		}
	}

	return k;
}

bool string_repetition( char* dst, const char *src, const size_t len, const size_t num )
{
	if ( !src || 0 == len || 0 == num )  { return FALSE; }
	// O(l*n)
	for ( size_t i = 0; i < len; i++ )  {
		for ( size_t j = 0; j < num; j++ )  {
			size_t offset = (len*j) + i;
			dst[offset] = src[i];
		}
	}

	return TRUE;
}

bool string_concatenate( char* dst, const char *a, const size_t a_l, const char *b, const size_t b_l )
{
	if ( !a || !b || 0 == a_l || 0 == b_l )  { return FALSE; }

	size_t n = b_l;
	if ( a_l > b_l )  { n = a_l; }

	for ( size_t i = 0; i < n; i++ )  {
		if ( i < a_l - 1 )  { dst[i] = a[i]; }
		if ( i < b_l )  { dst[a_l - 1 + i] = b[i]; }
	}

	return TRUE;
}

// Converts the passed string into a integer
// \param str The string that contains numbers
// \param converted_value the value converted from the passed string str
// \return true for a successful conversion, else false
bool string_to_int( const char *s, int *v )
{
	if ( !s || !v )  { return FALSE; }

	return TRUE;
}

char* string_generate( const char *src, size_t *len )
{
	if ( !src || !len || 0 == *len )  { return NULL; }

	size_t size = ( sizeof( char ) * (*len) );
	size_t term = src[*len];
	if ( '\0' != src[*len] )  {
		size++;
		(*len)++;
		term = '\0';
	}

	char *dst = malloc( size );
	if( !dst )  { return NULL; }
	string_duplicate(dst, src, *len);
	dst[*len] = term;

	return dst;
}
