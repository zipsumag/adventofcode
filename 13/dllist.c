#include <stdlib.h>
#include <stdio.h>
#include "dllist.h"

/* TODO: need a workaround for freeing up data in nodes */

static struct dllist_node* dllist_node_create(void* data)
{
  struct dllist_node* n = (struct dllist_node*)malloc(sizeof(struct dllist_node));
  n->data = data;
  n->next = NULL;
  n->previous = NULL;
  return n;
}

void dllist_push_back(struct dllist* dll, void* data)
{
  struct dllist_node* n;
  n = dllist_node_create(data);
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

void dllist_push_front(struct dllist* dll, void* data)
{
  struct dllist_node* n;
  n = dllist_node_create(data);
  if (!dll->begin) {
    dll->begin = n;
    dll->end = n;
  } else { 
    n->next = dll->begin;
    dll->begin->previous = n;
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
    /*free(dll->end->data);*/
    free(dll->end);
    dll->begin = NULL;
    dll->end = NULL;
  } else {
    tmp = dll->end->previous;
    /*free(dll->end->data);*/
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
    /*free(dll->begin->data);*/
    free(dll->begin);
    dll->begin = NULL;
    dll->end = NULL;
  } else {
    tmp = dll->begin->next;
    /*free(dll->begin->data);*/
    free(dll->begin);
    dll->begin = tmp;
    dll->begin->previous = NULL;
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
    /*for (i = 0; i < idx; ++i) tmp = tmp->next;*/
    tmp = dllist_idx(dll, idx);
    n = dllist_node_create(data);
    n->next = tmp->next;
    n->previous = tmp;
    tmp->next = n;
    n->next->previous = n;
    ++dll->length;
  }
}

struct dllist_node* dllist_idx(struct dllist* dll, int idx)
{
  int i;
  struct dllist_node* l = dll->begin;
  if (dll->length <= idx) return NULL;
  for (i = 0; i < idx; ++i) l = l->next;
  return l;
}

void dllist_remove(struct dllist* dll, int idx)
{
  struct dllist_node* p;
  if (idx >= dll->length) {
    return;
  } else if (idx == 0) { 
    dllist_pop_front(dll);
  } else if (idx == (dll->length - 1)) {
    dllist_pop_back(dll);
  } else {
    p = dllist_idx(dll, idx);
    p->previous->next = p->next;
    p->next->previous = p->previous;
    /*free(p->data);*/
    free(p);
    --dll->length;
  }
}

int dllist_find(struct dllist* dll, int** matchlist, void* data, int size)
{ 
  struct dllist_node* n = dll->begin;
  int i, j = 0, k = 0, *m = (int*)malloc(dll->length * sizeof(int));
  while (n) {
    for (i = 0; i < size; ++i) {
      if (!n->data && !data) 
        continue;
      else if (!data || !n->data || ((char*)n->data)[i] != ((char*)data)[i]) 
        break;
    }
    if (i == size) m[j++] = k;
    n = n->next;
    ++k;
  }
  *matchlist = (int*)malloc(k * sizeof(int));
  for (i = 0; i < j; ++i) (*matchlist)[i] = m[i];
  free(m);
  return j;
}

void dllist_reverse(struct dllist* dll)
{ 
  struct dllist_node* n = dll->end, *tmp;
  while (n) {
    tmp = n->previous;
    n->previous = n->next;
    n->next = tmp;
    n = n->next;
  } 
  tmp = dll->begin;
  dll->begin = dll->end; 
  dll->end = tmp;
}

void dllist_free(struct dllist* dll)
{ 
  struct dllist_node* n = dll->begin;
  while (n->next) {
    n = n->next;
    /*free(n->previous->data);*/
    free(n->previous);
  }
  /*free(n->data); */
  free(n); 
  dll->begin = NULL;
  dll->end = NULL;
}

void dllist_print(struct dllist* dll, void (*node_writer)(void*))
{ 
  struct dllist_node* n = dll->begin;
  while (n) {
    (*node_writer)(n->data); 
    n = n->next;
  }
}

