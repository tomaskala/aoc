#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SCORE 21
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

static int rolls[7] = {3, 4, 5, 6, 7, 8, 9};
static int freqs[10] = {0, 0, 0, 1, 3, 6, 7, 6, 3, 1};

static int
read_position(char buf[32])
{
  char *tok;
  size_t num;
  if (!(tok = strtok(buf, ":"))) {
    fprintf(stderr, "cannot tokenize the input\n");
    exit(EXIT_FAILURE);
  }
  if (!(tok = strtok(NULL, "\n"))) {
    fprintf(stderr, "cannot read the position token\n");
    exit(EXIT_FAILURE);
  }
  errno = 0;
  num = strtol(tok, NULL, 10);
  if (errno) {
    fprintf(stderr, "cannot read a number\n");
    exit(EXIT_FAILURE);
  }
  return (int) num;
}

static long
solve(int pos0, int pos1)
{
  size_t p0, np0, p1, np1, s0, ns0, s1, ns1, s, i, j;
  long visits[10][MAX_SCORE + 1][10][MAX_SCORE + 1] = {0}, w0 = 0, w1 = 0;
  visits[pos0][0][pos1][0] = 1;
  for (s0 = 0; s0 < MAX_SCORE; ++s0) {
    for (s1 = 0; s1 < MAX_SCORE; ++s1) {
      for (p0 = 0; p0 < 10; ++p0) {
        for (p1 = 0; p1 < 10; ++p1) {
          for (i = 0; i < 7; ++i) {
            np0 = (p0 + rolls[i]) % 10;
            ns0 = MIN(MAX_SCORE, s0 + np0 + 1);
            if (ns0 == MAX_SCORE)
              visits[np0][ns0][p1][s1] += freqs[rolls[i]]
                * visits[p0][s0][p1][s1];
            else {
              for (j = 0; j < 7; ++j) {
                np1 = (p1 + rolls[j]) % 10;
                ns1 = MIN(MAX_SCORE, s1 + np1 + 1);
                visits[np0][ns0][np1][ns1] += freqs[rolls[i]] * freqs[rolls[j]]
                  * visits[p0][s0][p1][s1];
              }
            }
          }
        }
      }
    }
  }
  for (p0 = 0; p0 < 10; ++p0) {
    for (p1 = 0; p1 < 10; ++p1) {
      for (s = 0; s < MAX_SCORE; ++s) {
        w0 += visits[p0][MAX_SCORE][p1][s];
        w1 += visits[p0][s][p1][MAX_SCORE];
      }
    }
  }
  return MAX(w0, w1);
}

int
main(void)
{
  char buf[32] = {'\0'};
  int pos0, pos1;
  long w;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "cannot read player 1 position\n");
    exit(EXIT_FAILURE);
  }
  pos0 = read_position(buf) - 1;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "cannot read player 2 position\n");
    exit(EXIT_FAILURE);
  }
  pos1 = read_position(buf) - 1;
  w = solve(pos0, pos1);
  printf("%ld\n", w);
  return EXIT_SUCCESS;
}
