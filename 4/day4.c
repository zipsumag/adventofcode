/* usage: ./day4 < input.dat                                                 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_NAME_LEN 100

int  idxsum = 0;
int  curr_idx;
int  north_pole_idx;
char crypted_name[MAX_NAME_LEN];
struct chardata {
  int  count;
  char letter;
};
struct chardata charcoll[1 + 'z' - 'a'];
enum state { INIT, CHARS, IDX, CHECK_NAME, CHECKSUM, NUM_STATES };

enum state init(void);
enum state get_chars(void);
enum state get_idx(void);
enum state check_name(void);
enum state check_checksum(void);

enum state (*const state_table[NUM_STATES])(void) = {
  init,
  get_chars,
  get_idx,
  check_name,
  check_checksum
};

enum state run_state(enum state curr_state) {
  return state_table[curr_state]();
}

int main(int argc, char** argv) {
  enum state curr_state = INIT;
  while (curr_state != NUM_STATES)
    curr_state = run_state(curr_state);
  printf("sum of the sector IDs of real rooms: %d\n", idxsum);
  printf("ID of nort pole room               : %d\n", north_pole_idx);
  return 0;
}

int chardata_count_cmp(const void* a, const void* b) {
  struct chardata* sa = (struct chardata*)a;
  struct chardata* sb = (struct chardata*)b;
  return (sb->count - sa->count);
  /*return (((struct chardata*)b)->count - ((struct chardata*)a)->count);*/
}

int chardata_letter_cmp(const void* a, const void* b) {
  struct chardata* sa = (struct chardata*)a;
  struct chardata* sb = (struct chardata*)b;
  if (sa->count == 1 && sb->count == 1) return (sa->letter - sb->letter);
  else if (sa->count == 1 && sb->count != 1) return (-1);
  else if (sa->count != 1 && sb->count == 1) return (1);
  else return 0;
  /*return (((struct chardata*)a)->letter - ((struct chardata*)b)->letter);*/
}

enum state init(void) {
  int i;
  for (i = 0; i < (1 + 'z' - 'a'); i++) {
    charcoll[i].letter = 'a' + i;
    charcoll[i].count  = 0;
  }
  for (i = 0; i < MAX_NAME_LEN; i++) crypted_name[i] = 0;
  curr_idx = 0;
  return CHARS;
}

enum state get_chars(void) { 
  int i = 0, suc;
  char c;
  for (suc = scanf("%c", &c); ((c >= 'a' && c <= 'z') || c == '-'); suc = scanf("%c", &c)) {
    crypted_name[i++] = c;
    if (c >= 'a' && c <= 'z') charcoll[c - 'a'].count++;
    if (suc == EOF) return NUM_STATES;
  }
  if (suc == EOF) return NUM_STATES;
  ungetc(c, stdin);
  return IDX;
}

enum state get_idx(void) {
  scanf("%d", &curr_idx);
  return CHECK_NAME;
}

enum state check_name(void) {
  regex_t regex;
  int     reti;
  reti = regcomp(&regex, "^.\\([a-z]\\)[a-z-]\\{4\\}\\1[a-z-]\\{3\\}\\1", 0);
  reti = regexec(&regex, crypted_name, 0, NULL, 0);
  if (!reti) north_pole_idx = curr_idx; 
  return CHECKSUM;
} 

enum state check_checksum(void) {
  int i, j = 0;
  char checksum_real[6], checksum_read[6];
  scanf("[%5[^]]]", checksum_read);
  getc(stdin);
  qsort(charcoll, (1 + 'z' - 'a'), sizeof(struct chardata), chardata_count_cmp);
  for (i = 0; i < 5; i++) {
    if (charcoll[i].count > 1) checksum_real[i] = charcoll[i].letter;
    else break;
  }
  if (i < 5)
    qsort(charcoll, (1 + 'z' - 'a'), sizeof(struct chardata), chardata_letter_cmp);
  for (; i < 5; i++) checksum_real[i] = charcoll[j++].letter;
  if (!strcmp(checksum_real, checksum_read)) idxsum += curr_idx;
  return INIT;
}
