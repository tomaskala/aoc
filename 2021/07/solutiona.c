#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMS 1000
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

int longcmp(const void *a, const void *b) {
  const long *ia = (long *) a, *ib = (long *) b;
  return *ia - *ib;
}

long cost(long pos, long positions[MAX_NUMS], size_t total) {
  size_t i;
  long result = 0;
  for (i = 0; i < total; ++i)
    result += ABS(positions[i] - pos);
  return result;
}

int main(void) {
  char buf[4000] = {'\0'}, *tok;
  size_t total = 0;
  long positions[MAX_NUMS] = {0}, curr, result;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "cannot read the input\n");
    exit(EXIT_FAILURE);
  }
  while ((tok = strtok(total ? NULL : buf, ","))) {
    errno = 0;
    curr = strtol(tok, NULL, 10);
    if (errno) {
      fprintf(stderr, "cannot parse a number\n");
      exit(EXIT_FAILURE);
    }
    if (total == MAX_NUMS) {
      fprintf(stderr, "%lu the input is too large\n", total);
      exit(EXIT_FAILURE);
    }
    positions[total++] = curr;
  }
  qsort(positions, total, sizeof(long), &longcmp);
  if (total % 2 == 0)
    result = MIN(cost(positions[total / 2], positions, total),
                 cost(positions[total / 2 - 1], positions, total));
  else
    result = cost(positions[total / 2], positions, total);
  printf("%ld\n", result);
  return EXIT_SUCCESS;
}
