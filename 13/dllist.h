#ifndef LISTH
#define LISTH

struct dllist {
  struct ll_node* begin;
  struct ll_node* end;
  int             length;
};

struct dllist_node {
  struct dll_node* next;
  struct dll_node* previous; 
  void*            data;
};

void dllist_push_back(struct dllist*, void *);
void dllist_push_front(struct dllist*, void *);
void dllist_pop_back(struct dllist*);
void dllist_pop_front(struct dllist*);
void dllist_insert(struct dllist*, int);
struct dllist_node* dllist_idx(struct dllist*, int);
void dllist_remove(struct dllist*, idx);
int dllist_find(struct dllist*, int*, void*); 
void dllist_reverse(struct dllist*);
void dllist_free(struct dllist*);

#endif
