#include <stdio.h>
#include <stdlib.h>
#include "../../dllist/dllist.h"

#define KRED  "\x1B[31m"
#define KWHT  "\x1B[37m"

struct mazepos {
  int x;
  int y;
};

struct dllist  history = { .begin = NULL, .end = NULL, .length = 0 };
struct dllist* backup; 
int            magic   = 1358;
struct mazepos target  = {31, 39};

int get_m_high_bits(int x)
{
  int n_high = 0;
  for (n_high = 0; x; ++n_high) x &= x - 1;
  return n_high;
}

int is_wall(const struct mazepos* pos)
{
  int n = magic;
  n += pos->x * (pos->x + 3 + 2 * pos->y);
  n += pos->y * (1 + pos->y);
  return (get_m_high_bits(n) & 1 ? 1 : 0);
}

int l1_norm(const struct mazepos* a, const struct mazepos* b)
{
  return (abs(a->x - b->x) + abs(a->y - b->y));
}

struct mazepos* path_backup(void)
{
  int i, path_size = history.length * sizeof(struct mazepos);
  struct mazepos* path = (struct mazepos*)malloc(path_size);
  struct dllist_node* n = history.begin;
  for (i = 0; i < history.length; ++i) {
    path[i].x = ((struct mazepos*)n->data)->x;
    path[i].y = ((struct mazepos*)n->data)->y;
    n = n->next;
  }
  return path;
}

void next_dir(struct mazepos* pos, int i)
{
  switch (i) {
    case 0:
      ++pos->x;
      break;
    case 1:
      ++pos->y;
      break;
    case 2:
      --pos->x;
      break;
    case 3:
      --pos->y;
      break;
  } 
}

void print_history(struct dllist* dll)
{
  struct mazepos p;
  for (p.y = 0; p.y <= target.y + 10; p.y++) {
    printf("%-3d ", p.y);
    for (p.x = 0; p.x < 2 * target.x; p.x++) {
      if (dllist_contains(dll, &p, sizeof(p))) 
        printf("%s%s%s", KRED, "O", KWHT);
      else 
        printf("%s", (is_wall(&p) ? "#" : "."));
    }
    printf("\n");
  }
}

struct mazepos* alloc_mazepos(const struct mazepos pos)
{
  struct mazepos* m;
  m = (struct mazepos*)malloc(sizeof(struct mazepos));
  m->x = pos.x;
  m->y = pos.y;
  return m;
}

void traverse(int* least_steps)
{
  int i;
  struct mazepos pos;
  if (((struct mazepos*)history.end->data)->x == target.x 
      && ((struct mazepos*)history.end->data)->y == target.y) {
    *least_steps = history.length - 1;
    dllist_free(backup);
    free(backup);
    backup = dllist_copy(&history, sizeof(struct mazepos));
    return;
  }
  if ((history.length - 1) >= *least_steps) return;
  pos = *((struct mazepos*)history.end->data);
  for (i = 0; i < 4; ++i, pos = *((struct mazepos*)history.end->data)) {
    next_dir(&pos, i);
    if (pos.x >= 0 && pos.y >= 0 
        && !is_wall(&pos) 
        && !dllist_contains(&history, &pos, sizeof(pos))
        && l1_norm(&pos, &target) < (*least_steps - (history.length - 1))) {
      dllist_push_back(&history, alloc_mazepos(pos), sizeof(pos));
      traverse(least_steps);
      dllist_pop_back(&history);
    }
  } 
}
 
/*void traverse_n(int n)*/
/*{*/
  /*int i;*/
  /*struct path_item* swp;*/
  /*struct mazepos pos;*/
  /*if (path_len == n) {*/
    /*printf(".");*/
    /*return;*/
  /*}*/
  /*for (i = 0, pos = *history->pos; i < 4; ++i, pos = *history->pos) {*/
    /*next_dir(&pos, i);*/
    /*if (pos.x >= 0 && pos.y >= 0*/
        /*&& !is_wall(&pos)*/
        /*&& !is_visited(pos)) {*/
      /*if (!is_visited(pos)) {*/
        /*dllist_push_back(backup, alloc_mazepos(pos), sizeof(pos));*/
      /*}*/
      /*dllist_push_back(&history, alloc_mazepos(pos), sizeof(pos));*/
      /*traverse_n(n);*/
      /*dllist_pop_back(&history);*/
    /*}*/
  /*}*/
/*}*/

int main(int argc, char** argv)
{
  struct mazepos start_pos = {1, 1};
  int least_steps = 100000;
  int max_steps = 50;

  dllist_push_back(&history, &start_pos, sizeof(start_pos));

  /* part 1                                                                   */
  traverse(&least_steps);
  print_history(backup);
  dllist_free(backup);
  free(backup);
  printf("Shortest path is %d steps\n", least_steps);

  /* part 2                                                                   */
  /*backup = dllist_copy(&history, sizeof(struct mazepos));*/
  /*traverse_n(max_steps);*/
  /*print_history(backup);*/
  /*dllist_free(backup);*/
  /*free(backup);*/
  /*printf("Number of blocks that can be visited within %d steps: %d\n", */
         /*max_steps, n_reachable);*/
  return 0;
}

 
