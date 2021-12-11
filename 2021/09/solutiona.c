#include <stdio.h>
#include <stdlib.h>

#define MMAX 102
#define NMAX 102
#define AT(i, j) ((i) * (NMAX) + (j))

int
main(void)
{
  char buf[128];
  size_t i, j, m = 1, n = 1;
  int risk = 0, matrix[MMAX * NMAX];
  for (i = 0; i < MMAX * NMAX; ++i)
    matrix[i] = 10;
  for (i = 1; fgets(buf, sizeof(buf), stdin); ++i, ++m) {
    if (m == MMAX - 1) {
      fprintf(stderr, "too many rows\n");
      exit(EXIT_FAILURE);
    }
    for (j = 1, n = 1; buf[j - 1] != '\n'; ++j, ++n) {
      if (n == NMAX - 1) {
        fprintf(stderr, "too many columns\n");
        exit(EXIT_FAILURE);
      }
      if (buf[j - 1] < '0' || '9' < buf[j - 1]) {
        fprintf(stderr, "cannot parse a number");
        exit(EXIT_FAILURE);
      }
      matrix[AT(i, j)] = buf[j - 1] - '0';
    }
  }
  m++;
  n++;
  for (i = 1; i < m - 1; ++i) {
    for (j = 1; j < n - 1; ++j) {
      if (matrix[AT(i, j)] < matrix[AT(i - 1, j)]
          && matrix[AT(i, j)] < matrix[AT(i + 1, j)]
          && matrix[AT(i, j)] < matrix[AT(i, j - 1)]
          && matrix[AT(i, j)] < matrix[AT(i, j + 1)]) {
          risk += matrix[AT(i, j)] + 1;
      }
    }
  }
  printf("%d\n", risk);
  return EXIT_SUCCESS;
}
