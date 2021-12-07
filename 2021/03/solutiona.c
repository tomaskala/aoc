#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define LONG_WIDTH (int)(sizeof(long) * 8)

int
main(void)
{
  char buf[32] = {'\0'}, *p;
  int i, lines = 0, counter[LONG_WIDTH] = {0};
  long curr, excess, gamma = 0;
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      errno = 0;
      curr = strtol(buf, &p, 2);
      if (errno) {
        fprintf(stderr, "error parsing %s", buf);
        exit(EXIT_FAILURE);
      }
      lines++;
      for (i = 0; i < LONG_WIDTH; ++i)
        counter[i] += (curr >> i) & 1;
    }
  }
  excess = LONG_WIDTH - (p - buf);
  for (i = LONG_WIDTH; i >= 0; --i)
    gamma = (gamma << 1 | (counter[i] > (lines >> 1)));
  printf("%ld\n", gamma * (~gamma << excess >> excess));
  return EXIT_SUCCESS;
}
