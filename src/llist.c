// Copyright (c) 2019 HG King Development and Design All Rights Reserved.
// llist.c file

#include <stdlib.h>
#include <stdio.h>
#include "mem.h"
#include "llist.h"

link_node_t *link_node_generate( void )
{
  link_node_t *node = malloc( sizeof( link_node_t ) );
  if ( !node )  { return NULL; }
  return node;
}

bool link_node_destroy( link_node_t *node )
{
  if ( !node )  { return FALSE; }

  if ( node->data_size && node->data )  { free( node->data ); }

  free( node );

  return TRUE;

}

link_node_t *link_node_initialize( link_node_t *node, void *data, size_t data_size )
{
  if ( !node )  { return NULL; }

  node->data = data;
  node->data_size = data_size;
  node->next = NULL;

  return node;
}

bool link_node_push_to_back( link_node_t *head, link_node_t *node )
{
  if ( !head || !node )  { return FALSE; }

  while ( head->next )  { head = head->next; }
  head->next = node;
  return TRUE;
}

bool link_node_push_to_front( link_node_t *head, link_node_t *node )
{
  if ( !node )  { return FALSE; }

  node->next = head;

  return TRUE;
}

link_node_t *link_node_pop_from_back( link_node_t *head )
{
  if ( !head )  { return NULL; }

  while ( head->next )  { head = head->next; }

  link_node_t *temp = head->next;
  head->next = NULL;
  return temp;
}
