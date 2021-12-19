#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MMAX 1000
#define NMAX 1000
#define AT(i, j) ((i) * (NMAX) + (j))
#define MAX2(x, y) ((x) > (y) ? (x) : (y))
#define MIN2(x, y) ((x) < (y) ? (x) : (y))

static long
readnum(char *buf)
{
  char *tok;
  long res;
  if (!(tok = strtok(buf, ","))) {
    fprintf(stderr, "invalid number token\n");
    exit(EXIT_FAILURE);
  }
  errno = 0;
  res = strtol(tok, NULL, 10);
  if (errno) {
    fprintf(stderr, "invalid number\n");
    exit(EXIT_FAILURE);
  }
  return res;
}

int
main(void)
{
  char buf[32] = {'\0'}, *tok1, *tok2;
  size_t x1, x2, y1, y2, i, j;
  int *mat = calloc(MMAX * NMAX, sizeof(int)), total = 0;
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      if (!(tok1 = strtok(buf, " -> "))) {
        fprintf(stderr, "invalid token1\n");
        exit(EXIT_FAILURE);
      }
      if (!(tok2 = strtok(NULL, " -> "))) {
        fprintf(stderr, "invalid token2\n");
        exit(EXIT_FAILURE);
      }
      x1 = (size_t) readnum(tok1);
      y1 = (size_t) readnum(NULL);
      x2 = (size_t) readnum(tok2);
      y2 = (size_t) readnum(NULL);
      if (x1 == x2)
        for (i = MIN2(y1, y2); i <= MAX2(y1, y2); ++i)
          mat[AT(i, x1)]++;
      else if (y1 == y2)
        for (i = MIN2(x1, x2); i <= MAX2(x1, x2); ++i)
          mat[AT(y1, i)]++;
      else if ((x1 < x2) == (y1 < y2))
        for (i = MIN2(y1, y2), j = MIN2(x1, x2);
             i <= MAX2(y1, y2) && j <= MAX2(x1, x2);
             ++i, ++j)
          mat[AT(i, j)]++;
      else
        for (i = MIN2(y1, y2), j = MAX2(x1, x2);
             i <= MAX2(y1, y2) && j >= MIN2(x1, x2);
             ++i, --j)
          mat[AT(i, j)]++;
    }
  }
  for (i = 0; i < MMAX * NMAX; ++i)
    total += mat[i] >= 2;
  printf("%d\n", total); 
  free(mat);
  return EXIT_SUCCESS;
}
