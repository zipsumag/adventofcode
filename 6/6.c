#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WORDLEN 20

struct chardata {
  char letter;
  int  count;
};

struct chardata wordstats[WORDLEN][1 + 'z' - 'a'];

int chardata_cmp_desc(const void* a, const void* b) {
  return ((*(struct chardata*)b).count - (*(struct chardata*)a).count);
}

int chardata_cmp_asc(const void* a, const void* b) {
  return ((*(struct chardata*)a).count - (*(struct chardata*)b).count);
}

int main(int argc, char** argv) {
  int i, j, k, part = (argc > 1 ? atoi(argv[1]) : 1);
  char word[WORDLEN + 1] = { 0 };
  int (*cmp_fun)(const void*, const void*) = (part > 1 ? chardata_cmp_asc : chardata_cmp_desc);
  while (fgets(word, WORDLEN, stdin)) { 
    word[strcspn(word, "\n")] = 0;
    for (i = 0; word[i]; i++) (wordstats[i][word[i] - 'a'].count)++;
  }
  for (j = 0; j < i; j++) for (k = 0; k < (1 + 'z' - 'a'); k++) wordstats[j][k].letter = 'a' + k;
  for (j = 0; j < i; j++) {
    qsort(&wordstats[j][0], (1 + 'z' - 'a'), sizeof(struct chardata), cmp_fun);
    printf("%c", wordstats[j][0].letter);
  }
  return 0; 
}
