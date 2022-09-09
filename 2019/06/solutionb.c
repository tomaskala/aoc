#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define EXP 15

struct ht_entry {
  uint64_t key;
  uint64_t val;
  int visited;
};

struct ht {
  struct ht_entry ht[1 << EXP];
  int32_t len;
};

void ht_init(struct ht *tbl)
{
  tbl->len = 0;
  for (int i = 0; i < 1 << EXP; ++i) {
    tbl->ht[i].key = 0;
    tbl->ht[i].val = 0;
    tbl->ht[i].visited = 0;
  }
}

int32_t ht_lookup(uint64_t hash, int exp, int32_t idx)
{
  uint32_t mask = ((uint32_t) 1 << EXP) - 1;
  uint32_t step = (hash >> (64 - exp)) | 1;
  return (idx + step) & mask;
}

void ht_insert(struct ht *tbl, uint64_t keyhash, uint64_t valhash)
{
  for (int32_t i = keyhash;;) {
    i = ht_lookup(keyhash, EXP, i);
    if (tbl->ht[i].key == 0 && tbl->ht[i].val == 0) {
      if ((uint32_t) tbl->len + 1 == (uint32_t) 1 << EXP) {
        fprintf(stderr, "Table out of memory\n");
        return;
      }
      tbl->ht[i].key = keyhash;
      tbl->ht[i].val = valhash;
      tbl->len++;
      return;
    }
  }
}

struct ht_entry *ht_get(struct ht *tbl, uint64_t keyhash)
{
  for (int32_t i = keyhash;;) {
    i = ht_lookup(keyhash, EXP, i);
    if (tbl->ht[i].key == keyhash && tbl->ht[i].val != 0)
      return &tbl->ht[i];
  }
  return NULL;
}

uint64_t fnv1a(char **buf)
{
  char *s = *buf;
  uint64_t h = 0x3243f6a8885a308d;
  for (; *s != '\0' && *s != ')' && *s != '\n'; ++s) {
    h ^= *s & 255;
    h *= 1111111111111111111;
  }
  *buf = s;
  return h ^ h>>32;
}

int main(void)
{
  char buf[16] = {'\0'};
  struct ht tbl;
  ht_init(&tbl);
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      char *p = buf;            // "ABC)DEF"
      uint64_t h1 = fnv1a(&p);  // hash("ABC")
      p++;                      // skip ')'
      uint64_t h2 = fnv1a(&p);  // hash("DEF")
      ht_insert(&tbl, h2, h1);  // store h("DEF")->h("ABC")
    }
  }
  char *goal = "COM";
  char *you = "YOU";
  char *san = "SAN";
  uint64_t goalhash = fnv1a(&goal);
  uint64_t youhash = fnv1a(&you);
  uint64_t sanhash = fnv1a(&san);
  uint64_t current;
  struct ht_entry *current_entry;
  int result = 0;
  int common = 0;
  current = youhash;
  while (current != goalhash) {
    current_entry = ht_get(&tbl, current);
    current_entry->visited = 1;
    current = current_entry->val;
    result++;
  }
  current = sanhash;
  while (current != goalhash) {
    current_entry = ht_get(&tbl, current);
    current = current_entry->val;
    result++;
    if (current_entry->visited)
      common++;
  }
  printf("%d\n", result - 2 * common - 2);
  return EXIT_SUCCESS;
}
