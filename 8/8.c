#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct operation {
  const char* match_str;
  const int   match_num;
  void (*handler)(int, int);
};

char screen[6][50] = { 0 };

int get_lit_count(void) {
  int i, j, lit_count = 0;
  for (i = 0; i < 6; ++i)
    for (j = 0; j < 50; ++j)
      if (screen[i][j]) ++lit_count;
  return lit_count;
}

void print_screen(void) {
  int i, j;
  for (i = 0; i < 6; ++i) {
    for (j = 0; j < 50; ++j) 
      printf("%s", (screen[i][j] ? "#" : "."));
    printf("\n");
  }
}

void rect(int a, int b) { 
  int i, j;
  for (i = 0; i < a; ++i) for (j = 0; j < b; ++j) screen[j][i] = 1;
}

void rot_row(int row, int num) { 
  int i;
  char temp_row[50];
  for (i = 0; i < 50; ++i) temp_row[i] = screen[row][i];
  for (i = 0; i < 50; ++i) screen[row][(i + num) % 50] = temp_row[i]; 
}

void rot_col(int col, int num) { 
  int i;
  char temp_col[6];
  for (i = 0; i < 6; ++i) temp_col[i] = screen[i][col];
  for (i = 0; i < 6; ++i) screen[(i + num) % 6][col] = temp_col[i]; 
}

struct operation operations[] = { 
  { "rect %dx%d\n",               4, rect    }, 
  { "rotate row y=%d by %d\n",    8, rot_row },
  { "rotate column x=%d by %d\n", 8, rot_col }
};

int main(int argc, char** argv) {
  int i, a = 0, b = 0;
  char buf[31];
  while (fgets(buf, 30, stdin)) {
    for (i = 0; i < 3; ++i) {
      if (0 == strncmp(buf, operations[i].match_str, operations[i].match_num)) { 
        sscanf(buf, operations[i].match_str, &a, &b);
        operations[i].handler(a, b); 
        break;
      } 
    } 
  } 
  printf("Number of pixels lit: %d\n", get_lit_count());
  print_screen();
  return 0;
}

