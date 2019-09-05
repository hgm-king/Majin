// Copyright (c) 2019 HG King Development and Design All Rights Reserved.
// tree.h file

#ifndef MAJIN_TREE_H
#define MAJIN_TREE_H


typedef struct tree_node {
  int data;   // pointer to data aka address
  struct tree_node *left;
  struct tree_node *right;
} tree_node_t;

tree_node_t *tree_node_generate();
bool tree_node_initialize( tree_node_t *node, int value );
tree_node_t *tree_insert( tree_node_t *head, int value );
tree_node_t *tree_node_create( int value );
void tree_destroy( tree_node_t *head);
void tree_node_destroy( tree_node_t *node );
void tree_inorder_walk( tree_node_t *head );
void tree_postorder_walk( tree_node_t *head );
void tree_preorder_walk( tree_node_t *head );


#endif
