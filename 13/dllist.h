#ifndef LISTH
#define LISTH

struct dllist {
  struct dllist_node* begin;
  struct dllist_node* end;
  int                 length;
};

struct dllist_node {
  struct dllist_node* next;
  struct dllist_node* previous; 
  void*               data;
};

/* push to the end of the list                                                */
void dllist_push_back(struct dllist*, void*);

/* pus to the front of the list                                               */
void dllist_push_front(struct dllist*, void*);

/* pop from the end of the list                                               */
void dllist_pop_back(struct dllist*);

/* pop from the beginning of the list                                         */
void dllist_pop_front(struct dllist*);

/* insert to a specified index of the list                                    */
void dllist_insert(struct dllist*, int, void*);

/* get pointer of the node in the list at a specified positio, returns NULL   */
/* if idx is outside the range of the list                                    */ 
struct dllist_node* dllist_idx(struct dllist*, int);

/* removes node from the list from a specified position                       */ 
void dllist_remove(struct dllist*, int);

/* collect indices of nodes in the list of which the data field equals to the */ 
/* specified data (with specified size in the last argument), returns the     */ 
/* number of elements found                                                   */ 
/* can find empty elements (NULL should be passed as data pointer in this     */ 
/* case) resulting from dynamic extension of the list                         */ 
int dllist_find(struct dllist*, int**, void*, int);

/* reverse the order of the list                                              */ 
void dllist_reverse(struct dllist*);

/* free the whole list (except for the dllist structure)                      */ 
void dllist_free(struct dllist*);

/* prints the contnets to the standard input using the passed function for    */ 
/* writing the data of nodes                                                  */ 
void dllist_print(struct dllist*, void (*)(void*));

#endif
