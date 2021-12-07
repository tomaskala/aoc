#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PERIOD 9
#define SIMS 256

int
main(void)
{
  char buf[800] = {'\0'}, *tok;
  size_t i = 0, head = 0;
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
    curr = fish[head];
    head = (head + 1) % PERIOD;
    fish[(head + 6) % PERIOD] += curr;
    fish[(head + 8) % PERIOD] = curr;
  }
  for (i = 0; i < PERIOD; ++i)
    total += fish[i];
  printf("%lu\n", total);
  return EXIT_SUCCESS;
}
