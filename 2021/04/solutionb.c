#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOARDS 100
#define MAX_NUMS 256
#define M 6
#define N 6
#define AT(b, i, j) ((b) * (M) * (N) + (i) * (N) + (j))

int
main(void)
{
  char buf[512] = {'\0'}, *tok;
  int done[MAX_BOARDS] = {0};
  size_t b, i, j = 0, n, totalnums = 0, totalboards = 0, boardsdone = 0;
  long nums[MAX_NUMS] = {0}, boards[MAX_BOARDS * N * N] = {0}, curr;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "cannot read the numbers\n");
    exit(EXIT_FAILURE);
  }
  tok = strtok(buf, ",");
  while (1) {
    errno = 0;
    nums[totalnums++] = strtol(tok, NULL, 10);
    if (errno) {
      fprintf(stderr, "error parsing the numbers\n");
      exit(EXIT_FAILURE);
    }
    if (!(tok = strtok(NULL, ",")))
      break;
  }
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      if (buf[0] == '\n') {
        for (i = 0; i < N; ++i) boards[j++] = 1;
        totalboards++;
        continue;
      }
      tok = strtok(buf, " ");
      boards[j++] = 1;
      while (1) {
        errno = 0;
        curr = strtol(tok, NULL, 10);
        if (errno) {
          fprintf(stderr, "error parsing the board\n");
          exit(EXIT_FAILURE);
        }
        boards[j++] = curr;
        if (!(tok = strtok(NULL, " ")))
          break;
      }
    }
  }
  for (n = 0; n < totalnums; ++n)
    for (b = 0; b < totalboards; ++b) {
      if (done[b]) continue;
      for (i = 1; i < M; ++i)
        for (j = 1; j < N; ++j)
          if (nums[n] == boards[AT(b, i, j)]) {
            boards[AT(b, i, j)] = -1;
            if (++boards[AT(b, i, 0)] == N || ++boards[AT(b, 0, j)] == M) {
              done[b] = 1;
              if (++boardsdone == totalboards)
                goto done;
            }
          }
    }
done:
  curr = 0;
  for (i = 1; i < M; ++i)
    for (j = 1; j < N; ++j)
      if (boards[AT(b, i, j)] > -1)
        curr += boards[AT(b, i, j)];
  printf("%ld\n", nums[n] * curr);
  return EXIT_SUCCESS;
}
