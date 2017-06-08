#include <stdio.h>
#include <stdlib.h>

#define KRED  "\x1B[31m"
#define KWHT  "\x1B[37m"

struct mazepos {
  int x;
  int y;
};

struct path_item {
  struct mazepos*   pos;
  struct path_item* previous;
};

int               magic    = 1358;
struct path_item* history  = NULL; 
struct path_item* backup   = NULL;
int               path_len = -1;
struct mazepos    target   = {31, 39};

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

int is_visited(struct mazepos pos)
{
  struct path_item* step = history;
  while (step) {
    if (step->pos->x == pos.x && step->pos->y == pos.y) return 1;
    step = step->previous;
  }
  return 0;
}

struct path_item* path_push(struct mazepos pos)
{
  struct path_item* tmp;
  tmp = history;
  history = (struct path_item*)malloc(sizeof(struct path_item));
  history->pos = (struct mazepos*)malloc(sizeof(struct mazepos));
  history->pos->x = pos.x;
  history->pos->y = pos.y;
  history->previous = tmp; 
  path_len++;
  return history;
}

struct path_item* path_pop(void)
{
  struct path_item* tmp;
  tmp = history;
  history = history->previous;
  free(tmp->pos);
  free(tmp);
  path_len--;
  return history;
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

void print_history(void)
{
  struct mazepos p;
  for (p.y = 0; p.y <= target.y + 10; p.y++) {
    printf("%-3d ", p.y);
    for (p.x = 0; p.x < 2 * target.x; p.x++) {
      if (is_visited(p)) printf("%s%s%s", KRED, "O", KWHT);
      else printf("%s", (is_wall(&p) ? "#" : "."));
    }
    printf("\n");
  }
}

void write_history(void)
{
  struct path_item* p = history;
  while (p->previous) {
    printf("(%d, %d) <- ", p->pos->x, p->pos->y);
    p = p->previous;
  }
  printf("(%d, %d)", p->pos->x, p->pos->y);
}

void traverse(int* least_steps)
{
  int i;
  struct mazepos pos;
  if (history->pos->x == target.x && history->pos->y == target.y) {
    /* TODO: how should alternative solutions be handled? */
    /*if (path_len < *least_steps) */
    *least_steps = path_len;
    /*print_history();*/
    return;
  }
  if (path_len >= *least_steps) return;
  /* TODO: equally long paths are considered the same                        */
  /* TODO: sort the directions in the order of the resulting block's L1      */
  /* distance                                                                */
  for (i = 0, pos = *history->pos; i < 4; ++i, pos = *history->pos) {
    next_dir(&pos, i);
    if (pos.x >= 0 && pos.y >= 0 
        && !is_wall(&pos) 
        && !is_visited(pos) 
        && l1_norm(&pos, &target) < (*least_steps - path_len)) {
      path_push(pos);
      traverse(least_steps);
      path_pop();
    }
  } 
}
 
int main(int argc, char** argv)
{
  struct mazepos start_pos = {1, 1};
  int least_steps = 100000;
  history = path_push(start_pos);
  traverse(&least_steps);
  printf("Shortest path is %d steps\n", least_steps);
  return 0;
}

 
