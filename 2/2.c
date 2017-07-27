#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int    pos[2];

struct crypting {
  int  part;
  void (*repos)(char);
  char (*getpos)(void);
};

char   allowed_places[][5] = {
  {  0,   0,  '1',  0,   0  },
  {  0,  '2', '3', '4',  0  },
  { '5', '6', '7', '8', '9' },
  {  0,  'A', 'B', 'C',  0  },
  {  0,   0,  'D',  0,   0  }
};

void init_pos(int part) {
  switch (part) {
  case 1:
    pos[0] = 1;
    pos[1] = 1;
    break;
  case 2:
    pos[0] = 2;
    pos[1] = 0;
    break;
  }
}

void repos_parttwo(char mov) {
  int idx = 0, canmov = -1;
  switch (mov) {
  case 'L':
    idx    = 1;
    canmov = (pos[1] > 0 && allowed_places[pos[0]][pos[1] - 1] > 0);
  case 'U':
    canmov   = (canmov == -1 ? (pos[0] > 0 && allowed_places[pos[0] - 1][pos[1]] > 0) : canmov);
    pos[idx] = (canmov ? pos[idx] - 1 : pos[idx]);
    break;
  case 'R':
    idx    = 1;
    canmov = (pos[1] < 4 && allowed_places[pos[0]][pos[1] + 1] > 0);
  case 'D':
    canmov   = (canmov == -1 ? (pos[0] < 4 && allowed_places[pos[0] + 1][pos[1]] > 0) : canmov);
    pos[idx] = (canmov ? pos[idx] + 1 : pos[idx]);
    break;
  }
}

void repos_partone(char mov) {
  int idx = 0;
  switch (mov) {
    case 'L':
      idx = 1;
    case 'U':
      pos[idx] = (pos[idx] > 0 ? pos[idx] - 1 : pos[idx]);
      break;
    case 'R':
      idx = 1;
    case 'D':
      pos[idx] = (pos[idx] < 2 ? pos[idx] + 1 : pos[idx]);
      break;
  }
}

char getpos_partone(void) {
  return ('0' + 1 + pos[1] + 3 * pos[0]);
}

char getpos_parttwo(void) { 
  return (allowed_places[pos[0]][pos[1]]);
}

struct crypting cryptings[] = {
  { 1, repos_partone, getpos_partone },
  { 2, repos_parttwo, getpos_parttwo }
};

int main(int argc, char** argv) {
  int  i = 0, j;
  char mov, code[20];
  struct crypting c = cryptings[(argc > 1 ? (atoi(argv[1]) > 1 ? 1 : 0) : 0)];

  init_pos(c.part);
  while (scanf("%c", &mov) != EOF) {
    for (; mov != '\n'; scanf("%c", &mov)) c.repos(mov);
    code[i++] = c.getpos();
  }
  printf("the code is: ");
  for (j = 0; j < i; j++) printf("%c", code[j]);
  puts("\n");
  return 0;
}
