#include <dllist.h>
#include <stdlib.h>
#include <stdio.h>


static struct dllist_node* dll_node_create(void* data)
{
  struct dllist_node* n = (struct dllist_node*)malloc(sizeof(struct dllist_node));
  n->data = data;
  n->next = NULL;
  n->previous = NULL;
  return n;
}

void dllist_push_back(struct dllist* dll, void * data)
{
  struct dllist_node* n;
  n = dll_node_create(data);
  n->previous = dll->end;
  if (!dll->begin) {
    dll->begin = n;
    dll->end = n;
  } else {
    dll->end->next = n;
    dll->end = n;
  }
  ++dll->length;
}

void dllist_push_front(struct dllist* dll, void * data)
{
  struct dllist_node* n;
  n = dll_node_create(data);
  if (!dll->begin) {
    dll->begin = n;
    dll->end = n;
  } else { 
    n->next = dll->begin;
    dll->begin = n;
  }
  ++dll->length;
}

void dllist_pop_back(struct dllist* dll)
{
  struct dllist_node* tmp;
  if (!dll->end) { 
    return;
  } else if (dll->begin == dll->end) {
    free(dll->end->data);
    free(dll->end);
    dll->begin = NULL;
    dll->end = NULL;
  } else {
    tmp = dll->end->previous;
    free(dll->end->data);
    free(dll->end);
    tmp->next = NULL;
    dll->end = tmp;
  }
  --dll->length;
}

void dllist_pop_front(struct dllist* dll)
{
  struct dllist_node* tmp;
  if (!dll->begin) { 
    return;
  } else if (dll->begin == dll->end) {
    free(dll->begin->data);
    free(dll->begin);
    dll->begin = NULL;
    dll->end = NULL;
  } else {
    tmp = dll->begin->next;
    free(dll->begin->data);
    free(dll->begin);
    dll->begin = tmp;
  }
  --dll->length;
}

void dllist_insert(struct dllist* dll, int idx, void* data)
{ 
  struct dllist_node* tmp = dll->begin, *n;
  /* dynamically resizable */
  if (idx > dll->length) {
    /* TODO: all could be mallocced at the same time */
    while (idx > dll->length) dllist_push_back(dll, NULL);
    dllist_push_back(dll, data); 
  } else if (idx == 0) {
    dllist_push_front(dll, data);
  } else if (idx == dll->length) {
    dllist_push_back(dll, data);
  } else {
    /* TODO: use dllist_idx here */
    for (i = 0; i < idx; ++i) tmp = tmp->next;
    n = dll_node_create(data);
    n->next = tmp->next;
    n->previous = tmp;
    tmp->next = n;
    n->next->previous = n;
  }
}

struct dllist_node* dllist_idx(struct dllist* dll, int idx)
{
  int i;
  struct dllist_node* l;
  if (dll->length <= idx) return NULL;
  for (i = 0; i < idx; ++i) l = l->next;
  return l->next;
}

void dllist_remove(struct dllist*, idx);
int dllist_find(struct dllist*, int*, void*); 
void dllist_reverse(struct dllist*);
void dllist_free(struct dllist*);

