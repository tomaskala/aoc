#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MMAX 140
#define NMAX 140
#define AT(i, j) ((i) * NMAX + (j))

int
main(void)
{
  char buf[256] = {'\0'};
  char *matrix = calloc(MMAX * NMAX, sizeof(char));
  char *work = calloc(MMAX * NMAX, sizeof(char));
  int changed, steps = 0;
  size_t i, j, next, m, n = 0;
  for (m = 0; fgets(buf, sizeof(buf), stdin); ++m) {
    if (m == MMAX) {
      fprintf(stderr, "too many lines of input\n");
      exit(EXIT_FAILURE);
    }
    for (n = 0; buf[n] != '\n' && buf[n] != '\0'; ++n) {
      if (n == NMAX) {
        fprintf(stderr, "too many columns of input\n");
        exit(EXIT_FAILURE);
      }
      matrix[AT(m, n)] = work[AT(m, n)] = buf[n];
    }
  }
  do {
    changed = 0;
    for (i = 0; i < m; ++i) {
      for (j = 0; j < n; ++j) {
        if (matrix[AT(i, j)] == '>') {
          next = j + 1;
          if (next == n)
            next = 0;
          if (matrix[AT(i, next)] == '.') {
            changed = 1;
            work[AT(i, j)] = '.';
            work[AT(i, next)] = '>';
          }
        }
      }
    }
    memcpy(matrix, work, MMAX * NMAX * sizeof(char));
    for (i = 0; i < m; ++i) {
      for (j = 0; j < n; ++j) {
        if (work[AT(i, j)] == 'v') {
          next = i + 1;
          if (next == m)
            next = 0;
          if (work[AT(next, j)] == '.') {
            changed = 1;
            matrix[AT(i, j)] = '.';
            matrix[AT(next, j)] = 'v';
          }
        }
      }
    }
    memcpy(work, matrix, MMAX * NMAX * sizeof(char));
    steps++;
  } while (changed);
  free(matrix);
  free(work);
  printf("%d\n", steps);
  return EXIT_SUCCESS;
}
