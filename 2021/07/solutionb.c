#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMS 1000
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

static long
cost(long pos, long positions[MAX_NUMS], size_t total)
{
  size_t i;
  long diff, result = 0;
  for (i = 0; i < total; ++i) {
    diff = ABS(positions[i] - pos);
    result += diff * (diff + 1);
  }
  return result / 2;
}

int
main(void)
{
  char buf[4000] = {'\0'}, *tok;
  size_t i, total = 0;
  long positions[MAX_NUMS] = {0}, curr, result;
  double mean = 0.0;
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
  for (i = 0; i < total; ++i)
    mean += positions[i];
  mean /= total;
  result = MIN(cost((size_t) (mean - 0.5), positions, total),
               cost((size_t) (mean + 0.5), positions, total));
  printf("%ld\n", result);
  return EXIT_SUCCESS;
}
