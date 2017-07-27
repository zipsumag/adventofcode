#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int  disk_size, filled   = 0;
char *checksum, *disk;


void fill_with_data(void) {
  int i, prev_end;
  while (filled < disk_size) {
    prev_end = filled;
    if (filled < disk_size) disk[filled++] = 0;
    for (i = prev_end - 1; i >= 0; --i) {
      if (filled < disk_size) disk[filled++] = !disk[i];
      else break; 
    }
  }
} 

int get_checksum(void) {
  int i, j = 0, prev_len = disk_size;
  char* prev_sum = (char*)malloc(sizeof(char) * disk_size);
  for (i = 0; i < disk_size; ++i) {
    prev_sum[i] = disk[i];
    checksum[i] = disk[i];
  }
  while (!(j % 2)) {
    j = 0;
    for (i = 0; i < prev_len; ++i) prev_sum[i] = checksum[i];
    for (i = 0; i < prev_len; i += 2) {
      if (prev_sum[i] == prev_sum[i + 1]) checksum[j++] = 1;
      else checksum[j++] = 0;
    }
    prev_len = j;
  }
  free(prev_sum);
  return j;
} 

int main(int argc, char** argv) {
  int  j, checksum_len;
  char dig;

  disk_size = (argc > 1 ? (atoi(argv[1]) > 1 ? 35651584 : 272) : 272);
  checksum  = (char*)malloc(sizeof(char) * disk_size);
  disk      = (char*)malloc(sizeof(char) * disk_size);

  while (scanf("%c", &dig) == 1)
    if (dig == '1' || dig == '0') disk[filled++] = (dig == '0' ? 0 : 1);

  fill_with_data();

  checksum_len = get_checksum();
  for (j = 0; j < checksum_len; ++j) printf("%d", checksum[j]);

  free(checksum);
  free(disk);
  return 0; 
}


