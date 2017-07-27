#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int sides[9], possible = 0;


int intcmp(const void* a, const void* b) 
{
  return (*(int*)a - *(int*) b);
}

void get_possible(int part) 
{
  int i, j, tempsides[3];
  for (j = 0; j < 3; j++) {
    for (i = 0; i < 3; i++) 
      tempsides[i] = (part == 1 ? sides[3 * j + i] : sides[j + i * 3]);
    qsort(tempsides, 3, sizeof(int), intcmp);
    if ((tempsides[0] + tempsides[1]) > tempsides[2]) ++possible;
  }
  for (i = 0; i < 9; i++) sides[i] = 0;
}

int main(int argc, char** argv) 
{
  int i, success = 1, part = atoi(argv[1]);
  while (success != EOF) {
    for (i = 0; i < 9; i++) success = scanf("%d", &sides[i]);
    get_possible(part);
  }
  printf("number of possible triangles: %d\n", possible);
  return 0;
}

