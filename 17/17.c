#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../md5/md5.h"


struct coords {
  int x;
  int y;
};

struct coords target = { .x = 3, .y = 3 };

char* copy_pass(const char* pass)
{
  unsigned int len;
  char* p;
  len = strlen(pass);
  p = (char*)malloc(sizeof(char) * (len + 2));
  strcpy(p, pass);
  return p;
} 

void step(struct coords* pos, int dir, char* pass, char* md5string) {
  int len;
  if (!(md5string[dir] > 'a' && md5string[dir] < 'g')) {
    pos->x = target.x + 1;
    return;
  }
  len = strlen(pass);
  switch (dir) {
    case 0: /* UP                                                            */
      --pos->y;
      pass[len] = 'U';
      pass[len + 1] = '\0';
      break;
    case 1: /* DOWN                                                          */
      ++pos->y;
      pass[len] = 'D';
      pass[len + 1] = '\0';
      break;
    case 2: /* LEFT                                                          */
      --pos->x;
      pass[len] = 'L';
      pass[len + 1] = '\0';
      break;
    case 3: /* RIGHT                                                         */
      ++pos->x;
      pass[len] = 'R';
      pass[len + 1] = '\0';
      break;
  } 
}

void traverse(struct coords pos, int depth, char* pass, int* path_len, char** path)
{
  int i;
  struct coords next_pos;
  char md5string[33], *new_pass;
  if (pos.x > target.x 
      || pos.x < 0 
      || pos.y > target.y 
      || pos.y < 0
      || depth > *path_len) { 
    return;
  } else if (pos.x == target.x && pos.y == target.y) {
    *path_len = depth;
    free(*path); 
    *path = malloc(sizeof(char) * (strlen(pass) + 1));
    strcpy(*path, pass);
    return;
  }
  new_pass = copy_pass(pass);
  md5sum_tostring(pass, md5string); 
  for (i = 0, next_pos = pos; i < 4; ++i, next_pos = pos) {
    step(&next_pos, i, new_pass, md5string); 
    /*printf("(%d, %d), %s\n", next_pos.x, next_pos.y, md5string);*/
    traverse(next_pos, depth + 1, new_pass, path_len, path);
    new_pass[strlen(pass)] = 0;
  }
  free(new_pass); 
}

int main(int argc, char** argv)
{
  int path_len = 50000;
  char pass[] = "pslxynzg";
  /*char pass[] = "ihgpwlah"; [> test case <]*/
  struct coords start_pos = { 0, 0 };
  char* path;
  traverse(start_pos, 0, pass, &path_len, &path);
  printf("shortest path: %s\n", path);
  return 0;
}
