#include <stdio.h>
#include <stdlib.h>

#define STEPS 2
#define IMG_MMIN (STEPS + 10)
#define IMG_NMIN (STEPS + 10)
#define IMG_MMAX 128
#define IMG_NMAX 128
#define MMAX (IMG_MMIN + IMG_MMAX + (STEPS + 10))
#define NMAX (IMG_MMIN + IMG_NMAX + (STEPS + 10))
#define AT(i, j) ((i) * (NMAX) + (j))

int
main(void)
{
  char buf[512 + 2] = {'\0'};
  size_t i, j, s, m, n = 0;
  int total = 0, *tmp;
  int *alg = malloc(512 * sizeof(int));
  int *matrix = calloc(MMAX * NMAX, sizeof(int));
  int *work = malloc(MMAX * NMAX * sizeof(int));
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "cannot read the algorithm\n");
    exit(EXIT_FAILURE);
  }
  for (i = 0; i < 512; ++i)
    alg[i] = buf[i] == '#';
  for (i = 0; i < MMAX * NMAX; ++i)
    work[i] = alg[0];
  fgets(buf, sizeof(buf), stdin);
  for (m = 0; fgets(buf, sizeof(buf), stdin); ++m)
    for (n = 0; buf[n] != '\n' && buf[n] != '\0'; ++n)
      matrix[AT(IMG_MMIN + m, IMG_NMIN + n)] = buf[n] == '#';
  for (s = 0; s < STEPS; ++s) {
    for (i = IMG_MMIN - s - 1; i < IMG_MMIN + m + s + 1; ++i) {
      for (j = IMG_NMIN - s - 1; j < IMG_NMIN + n + s + 1; ++j) {
        work[AT(i, j)] = alg[
            matrix[AT(i + 1, j + 1)] * (1 << 0)
          | matrix[AT(i + 1, j + 0)] * (1 << 1)
          | matrix[AT(i + 1, j - 1)] * (1 << 2)
          | matrix[AT(i + 0, j + 1)] * (1 << 3)
          | matrix[AT(i + 0, j + 0)] * (1 << 4)
          | matrix[AT(i + 0, j - 1)] * (1 << 5)
          | matrix[AT(i - 1, j + 1)] * (1 << 6)
          | matrix[AT(i - 1, j + 0)] * (1 << 7)
          | matrix[AT(i - 1, j - 1)] * (1 << 8)
        ];
      }
    }
    tmp = work;
    work = matrix;
    matrix = tmp;
  }
  for (i = IMG_MMIN - STEPS - 1; i < IMG_MMIN + m + STEPS + 1; ++i)
    for (j = IMG_NMIN - STEPS - 1; j < IMG_NMIN + n + STEPS + 1; ++j)
      total += matrix[AT(i, j)];
  free(alg);
  free(matrix);
  free(work);
  printf("%d\n", total);
  return EXIT_SUCCESS;
}
