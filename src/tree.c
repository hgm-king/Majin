#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "tree.h"

tree_node_t *tree_node_generate()
{
  tree_node_t *node = malloc( sizeof( tree_node_t ) );

  if ( !node )  { return NULL; }
  return node;
}

bool tree_node_initialize( tree_node_t *node, int value )
{
  if ( !node )  { return FALSE; }

  node->data = value;
  node->left = node->right = NULL;

  return TRUE;
}

tree_node_t *tree_insert( tree_node_t *head, int value )
{
  if ( !head )  { return tree_node_create( value ); }

  if ( head->data > value )  { head->left = tree_insert( head->left, value ); }
  else  { head->right = tree_insert( head->right, value ); }

  return head;
}

tree_node_t *tree_search( tree_node_t *head, int value )
{
  if ( !head || head->data == value )  { return head; }

  if ( head->data > value )  { return tree_search( head->left, value ); }
  else  { return tree_search( head->right, value ); }
}

tree_node_t *tree_node_create( int value )
{
  tree_node_t *node = tree_node_generate();
  if ( !tree_node_initialize( node, value ) )  { return NULL; }
  return node;
}

void tree_destroy( tree_node_t *head)
{
  if ( !head )  { return; }

  tree_destroy( head->left );
  tree_destroy( head->right );

  tree_node_destroy( head );
}

void tree_node_destroy( tree_node_t *node )
{
  if ( !node )  { return; }

  free( node );
}

void tree_inorder_walk( tree_node_t *head )
{
  if ( !head )  { return; }

  tree_inorder_walk( head->left );

  printf( "%d\n", head->data );

  tree_inorder_walk( head->right );
}

void tree_postorder_walk( tree_node_t *head )
{
  if ( !head )  { return; }

  tree_postorder_walk( head->left );
  tree_postorder_walk( head->right );

  printf( "%d\n", head->data );
}

void tree_preorder_walk( tree_node_t *head )
{
  if ( !head )  { return; }

  printf( "%d\n", head->data );

  tree_preorder_walk( head->left );
  tree_preorder_walk( head->right );
}
