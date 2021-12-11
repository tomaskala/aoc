#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MMAX 102
#define NMAX 102
#define AT(i, j) ((i) * (NMAX) + (j))

static int
dfs(int matrix[MMAX * NMAX], size_t i, size_t j, int visited[MMAX * NMAX])
{
  int result = 1;
  visited[AT(i, j)] = 1;
  if (!visited[AT(i - 1, j)] && matrix[AT(i - 1, j)] < 9)
    result += dfs(matrix, i - 1, j, visited);
  if (!visited[AT(i + 1, j)] && matrix[AT(i + 1, j)] < 9)
    result += dfs(matrix, i + 1, j, visited);
  if (!visited[AT(i, j - 1)] && matrix[AT(i, j - 1)] < 9)
    result += dfs(matrix, i, j - 1, visited);
  if (!visited[AT(i, j + 1)] && matrix[AT(i, j + 1)] < 9)
    result += dfs(matrix, i, j + 1, visited);
  return result;
}

int
main(void)
{
  char buf[128];
  size_t i, j, m = 1, n = 1;
  int basin, basins[3] = {0}, matrix[MMAX * NMAX], visited[MMAX * NMAX];
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
        memset(visited, 0x00, sizeof(visited));
        basin = dfs(matrix, i, j, visited);
        if (basin > basins[0]) {
          basins[2] = basins[1];
          basins[1] = basins[0];
          basins[0] = basin;
        } else if (basin > basins[1]) {
          basins[2] = basins[1];
          basins[1] = basin;
        } else if (basin > basins[2]) {
          basins[2] = basin;
        }
      }
    }
  }
  printf("%d\n", basins[0] * basins[1] * basins[2]);
  return EXIT_SUCCESS;
}
