#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MMAX 2048
#define NMAX 2048
#define AT(i, j) ((i) * (NMAX) + (j))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

static size_t
foldx(char *matrix, size_t m, size_t n, size_t coord)
{
  size_t i, j;
  for (i = 0; i < m; ++i)
    for (j = coord + 1; j < n; ++j)
      if (matrix[AT(i, j)] == '#')
        matrix[AT(i, coord - (j - coord))] = '#';
  return coord;
}

static size_t
foldy(char *matrix, size_t m, size_t n, size_t coord)
{
  size_t i, j;
  for (i = coord + 1; i < m; ++i)
    for (j = 0; j < n; ++j)
      if (matrix[AT(i, j)] == '#')
        matrix[AT(coord - (i - coord), j)] = '#';
  return coord;
}

int
main(void)
{
  char buf[2048] = {'\0'}, *tok, axis;
  char *matrix = malloc(MMAX * NMAX * sizeof(char));
  int dots = 1;
  size_t i, x, y, coord, mmax = 0, nmax = 0;
  for (i = 0; i < MMAX * NMAX; ++i)
    matrix[i] = '.';
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      if (buf[0] == '\n') {
        dots = 0;
        mmax++;
        nmax++;
        continue;
      }
      if (dots) {
        if (!(tok = strtok(buf, ","))) {
          fprintf(stderr, "cannot parse token1\n");
          exit(EXIT_FAILURE);
        }
        errno = 0;
        x = (size_t) strtol(tok, NULL, 10);
        if (errno) {
          fprintf(stderr, "cannot parse x\n");
          exit(EXIT_FAILURE);
        }
        if (!(tok = strtok(NULL, "\n"))) {
          fprintf(stderr, "cannot parse token2\n");
          exit(EXIT_FAILURE);
        }
        errno = 0;
        y = (size_t) strtol(tok, NULL, 10);
        if (errno) {
          fprintf(stderr, "cannot parse y\n");
          exit(EXIT_FAILURE);
        }
        matrix[AT(y, x)] = '#';
        mmax = MAX(mmax, y);
        nmax = MAX(nmax, x);
      } else {
        if (!(tok = strtok(buf, " "))) {
          fprintf(stderr, "cannot tokenize a fold\n");
          exit(EXIT_FAILURE);
        }
        if (!(tok = strtok(NULL, " "))) {
          fprintf(stderr, "cannot tokenize a fold\n");
          exit(EXIT_FAILURE);
        }
        if (!(tok = strtok(NULL, "="))) {
          fprintf(stderr, "cannot tokenize a fold axis\n");
          exit(EXIT_FAILURE);
        }
        axis = tok[0];
        if (!(tok = strtok(NULL, "\n"))) {
          fprintf(stderr, "cannot tokenize a fold coordinate\n");
          exit(EXIT_FAILURE);
        }
        errno = 0;
        coord = (size_t) strtol(tok, NULL, 10);
        if (errno) {
          fprintf(stderr, "cannot parse a fold coordinate\n");
          exit(EXIT_FAILURE);
        }
        switch (axis) {
        case 'x':
          nmax = foldx(matrix, mmax, nmax, coord);
          break;
        case 'y':
          mmax = foldy(matrix, mmax, nmax, coord);
          break;
        default:
          fprintf(stderr, "unexpected axis %c\n", axis);
          exit(EXIT_FAILURE);
        }
      }
    }
  }
  for (y = 0; y < mmax; ++y) {
    for (x = 0; x < nmax; ++x) {
      printf("%c", matrix[AT(y, x)]);
    }
    printf("\n");
  }
  free(matrix);
  return EXIT_SUCCESS;
}
