#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PERIOD 9
#define SIMS 80

int main(void) {
  char buf[800] = {'\0'}, *tok;
  size_t i = 0, j;
  long fish[PERIOD] = {0}, curr, total = 0;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "cannot read the input\n");
    exit(EXIT_FAILURE);
  }
  while ((tok = strtok(i ? NULL : buf, ","))) {
    i++;
    errno = 0;
    curr = strtol(tok, NULL, 10);
    if (errno) {
      fprintf(stderr, "cannot parse a number\n");
      exit(EXIT_FAILURE);
    }
    fish[curr]++;
  }
  for (i = 0; i < SIMS; ++i) {
    total = fish[0];
    for (j = 1; j < PERIOD; ++j)
      fish[j - 1] = fish[j];
    fish[6] += total;
    fish[8] = total;
  }
  total = 0;
  for (i = 0; i < PERIOD; ++i)
    total += fish[i];
  printf("%lu\n", total);
  return EXIT_SUCCESS;
}
