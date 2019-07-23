// Copyright (c) 2019 HG King Development and Design All Rights Reserved.
// llist.h file

#ifndef MAJIN_LINK_LISTS_H
#define MAJIN_LINK_LISTS_H


typedef struct link_node  {
  void *data;
  size_t data_size;
  struct link_node *next;
} link_node_t;

link_node_t *link_node_generate(void);
bool link_node_destroy(link_node_t*node);
link_node_t *link_node_initialize(link_node_t*node,void*data,size_t data_size);
bool link_node_push_to_back(link_node_t*head,link_node_t*node);
bool link_node_push_to_front(link_node_t*head,link_node_t*node);
link_node_t *link_node_pop_from_back(link_node_t*head);


#endif
