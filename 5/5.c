#include <stdio.h>
#include <stdlib.h>
#include "../md5/md5.h"

char password[9] = { 0 };
char door_id[10] = { 0 };

void get_key(char* buf, int idx) {
  int i;
  for (i = 0; door_id[i]; ++i) buf[i] = door_id[i];
  sprintf(&buf[i], "%d", idx);
}

void get_password(int part) {
  int i = 0, j, k, pass_idx = 0, valid;
  char temp[20], md5string[33];
  while (pass_idx < 8) {
    valid = 1;
    get_key(temp, i++);
    md5sum_tostring((unsigned char*)temp, (unsigned char*)md5string);
    for (j = 0; j < 5; ++j) if (md5string[j] != '0') break;
    if (j != 5) continue;
    switch (part) {
    case 1:
      password[pass_idx++] = md5string[5]; 
      break;
    case 2: 
      k      = (int)(md5string[5] - '0');
      valid &= (k >= 0 && k <= 7); 
      valid &= (password[k] == 0);
      if (valid) {
        password[k] = md5string[6];
        ++pass_idx;
      }
      break;
    }
  }
}

int main(int argc, char** argv) {
  int part = (argc > 1 ? (atoi(argv[1]) > 1 ? 2 : 1) : 1);
  scanf("%[^\n]", door_id); 
  get_password(part);
  printf("the password is: %s\n", password);
  return 0;
}
