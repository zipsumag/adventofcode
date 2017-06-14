#include <stdio.h>
#include <stdlib.h>
#include "dllist.h"


void write_data(void* d)
{
  if (d) printf("%d, ", *((int*)d));
  else printf("NULL, ");
}

int main(int argc, char** argv) 
{
  struct dllist l = {NULL, NULL, 0};
  int i, *data = (int*)malloc(4 * sizeof(int));
  for (i = 0; i < 4; ++i) data[i] = i;

  /* test push_back                                                           */
  for (i = 0; i < 4; ++i) dllist_push_back(&l, &data[i]);
  dllist_print(&l, &write_data);
  printf("\n"); 

  /* test push_front                                                          */
  dllist_push_front(&l, &data[2]);
  dllist_print(&l, &write_data);
  printf("\n"); 

  /* test pop_back                                                            */
  dllist_pop_back(&l);
  dllist_print(&l, &write_data);
  printf("\n"); 

  /* test pop_front                                                           */
  dllist_pop_front(&l);
  dllist_print(&l, &write_data);
  printf("\n"); 

  /* test insert (mid)                                                        */
  dllist_insert(&l, 1, &data[3]);
  dllist_print(&l, &write_data);
  printf("\n"); 

  /* test insert (front)                                                      */
  dllist_insert(&l, 0, &data[3]);
  dllist_print(&l, &write_data);
  printf("\n"); 

  /* test insert (back)                                                       */
  dllist_insert(&l, 5, &data[3]);
  dllist_print(&l, &write_data);
  printf("\n"); 

  /* test insert (beyond)                                                     */
  dllist_insert(&l, 8, &data[3]);
  dllist_print(&l, &write_data);
  printf("\n"); 

  /* test idx (2nd)                                                           */
  printf("2nd element: %d\n", *((int*)dllist_idx(&l, 1)->data));
  printf("1nd element: %d\n", *((int*)dllist_idx(&l, 0)->data));

  /* test remove (2nd)                                                        */
  dllist_remove(&l, 1);
  dllist_print(&l, &write_data);
  printf("\n"); 

  /* test remove (first)                                                      */
  dllist_remove(&l, 0);
  dllist_print(&l, &write_data);
  printf("\n"); 

  /* test remove (last)                                                       */
  dllist_remove(&l, l.length - 1);
  dllist_print(&l, &write_data);
  printf("\n"); 

  /* test find (elements with value 3)                                        */
  int match_count, *matches;
  match_count = dllist_find(&l, &matches, &data[3], sizeof(data[3]));
  printf("indices of nodes with value 3: ");
  for (i = 0; i < match_count; ++i) printf("%d, ", matches[i]);
  printf("\n"); 
  free(matches);

  /* test find (elements with value NULL)                                     */
  match_count = dllist_find(&l, &matches, NULL, sizeof(data[3]));
  printf("indices of nodes with value NULL: ");
  for (i = 0; i < match_count; ++i) printf("%d, ", matches[i]);
  printf("\n"); 
  free(matches);

  /* test reverse                                                             */
  dllist_reverse(&l);
  dllist_print(&l, &write_data);
  printf("\n"); 

  /* test reverse                                                             */
  dllist_free(&l);
  dllist_print(&l, &write_data);
  printf("\n"); 

  return 0; 
}
