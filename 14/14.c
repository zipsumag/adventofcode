#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../md5/md5.h"

#define MAX_KEY_LEN 20

char salt[10]   = { 0 };
int key_count   = 0;
int key_end_idx = 0;

struct item {
  struct item*  subsequent;
  unsigned char md5string[33];
};

struct hash_list {
  struct item* first;
  struct item* last;
} hash_list;

unsigned char has_triplet(const unsigned char* digest) { 
  int i;
  for (i = 0; digest[i + 2]; ++i) {
    if (digest[i] == digest[i + 1] && digest[i] == digest[i + 2])
      return digest[i];
  } 
  return 0;
}

unsigned char has_quintet(const unsigned char* digest, const unsigned char triplet) {
  int i, j, same;
  for (i = 0; digest[i + 4]; ++i) {
    if (digest[i] != triplet) continue;
    same = 1;
    for (j = 1; j < 5; ++j) {
      if (digest[i] == digest[i + j]) ++same;
      else break;
    }
    if (same == 5) return digest[i];
  } 
  return 0; 
}

void mkkey(char* key, const char* salt, const int index) { 
  int i;
  for (i = 0; salt[i]; ++i) key[i] = salt[i];
  sprintf(&key[i], "%d", index);
}

void hash_key(const char* key, unsigned char* md5string, int part) {
  int i;
  char tmp_md5string[33] = { 0 };
  md5sum_tostring((unsigned char*)key, md5string);
  if (part == 1) return;
  else for (i = 0; i < 2016; ++i) {
    strcpy(tmp_md5string, md5string);
    md5sum_tostring((unsigned char*)tmp_md5string, md5string);
  }
} 

struct item* get_next_hash(int part, int index) {
  struct item* item_head;
  char         key[MAX_KEY_LEN];
  item_head       = hash_list.first;
  hash_list.first = hash_list.first->subsequent;
  free(item_head);
  item_head                  = (struct item*)malloc(sizeof(struct item));
  hash_list.last->subsequent = item_head;
  hash_list.last             = item_head;
  hash_list.last->subsequent = NULL;
  mkkey(key, salt, key_end_idx++);
  hash_key(key, hash_list.last->md5string, part); 
  return hash_list.first;
}

void fill_hash_list(int part) {
  int i;
  char key[MAX_KEY_LEN];
  struct item* item_head;
  hash_list.first = (struct item*)malloc(sizeof(struct item));
  item_head       = hash_list.first;
  mkkey(key, salt, key_end_idx++);
  hash_key(key, item_head->md5string, part);
  for (i = 1; i <= 1000; ++i) {
    item_head->subsequent = (struct item*)malloc(sizeof(struct item));
    item_head             = item_head->subsequent;
    mkkey(key, salt, key_end_idx++);
    hash_key(key, item_head->md5string, part); 
  }
  hash_list.last             = item_head;
  hash_list.last->subsequent = NULL;
} 
    
int find_64th(int index, int part) {
  unsigned char triplet;
  struct item*  item_head;
  item_head = hash_list.first;
  if ((triplet = has_triplet(item_head->md5string)) != 0) {
    while (item_head->subsequent) {
      item_head = item_head->subsequent;
      if (has_quintet(item_head->md5string, triplet)) {
        ++key_count;
        break;
      }
    }
  } 
  (void)get_next_hash(part, index);
  if (key_count == 64) return 0;
  return 1;
}

int main(int argc, char** argv) {
  int i = -1, part;
  if (argc > 2) {
    strcpy(salt, argv[1]);
    part = (atoi(argv[2]) > 1 ? 2 : 1);
  } else {
    printf("PROVIDE SALT!"); /* jlmsuwbz */
    return 0;
  }
  fill_hash_list(part);
  while (find_64th(++i, part));
  printf("The 64th one-time pad key index: %d\n", i);
  return 0; 
}
