#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MMAX 10
#define NMAX 10
#define AT(i, j) ((i) * (NMAX) + (j))

static void
propagate(int matrix[MMAX * NMAX], size_t i, size_t j, size_t m, size_t n)
{
  size_t ii, jj;
  for (ii = i - (i > 0); ii < i + (i < m - 1) + 1; ++ii)
    for (jj = j - (j > 0); jj < j + (j < n - 1) + 1; ++jj)
      if (ii != i || jj != j)
        matrix[AT(ii, jj)]++;
}

int
main(void)
{
  char buf[16] = {'\0'};
  size_t s, i, j, m, n = 0, total = 0;
  int matrix[MMAX * NMAX], flashed[MMAX * NMAX], flashes;
  for (i = 0, m = 0; fgets(buf, sizeof(buf), stdin); ++i, ++m) {
    if (m == MMAX) {
      fprintf(stderr, "too many rows\n");
      exit(EXIT_FAILURE);
    }
    for (j = 0, n = 0; buf[j] != '\n'; ++j, ++n) {
      if (n == NMAX) {
        fprintf(stderr, "too many columns\n");
        exit(EXIT_FAILURE);
      }
      if (buf[j] < '0' || '9' < buf[j]) {
        fprintf(stderr, "cannot parse a number\n");
        exit(EXIT_FAILURE);
      }
      matrix[AT(i, j)] = buf[j] - '0';
    }
  }
  for (s = 0; total < m * n; ++s) {
    for (i = 0; i < MMAX * NMAX; ++i)
      matrix[i]++;
    memset(flashed, 0x00, sizeof(flashed));
    do {
      flashes = 0;
      for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
          if (!flashed[AT(i, j)] && matrix[AT(i, j)] > 9) {
            flashes++;
            flashed[AT(i, j)] = 1;
            propagate(matrix, i, j, m, n);
          }
        }
      }
    } while (flashes);
    for (total = 0, i = 0; i < MMAX * NMAX; ++i)
      if (matrix[i] > 9) {
        total++;
        matrix[i] = 0;
      }
  }
  printf("%lu\n", s);
  return EXIT_SUCCESS;
}
