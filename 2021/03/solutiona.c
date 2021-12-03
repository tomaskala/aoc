#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char buf[32] = {'\0'}, *p;
  int i, lines = 0, counter[64] = {0};
  long curr, ndigits, gamma = 0;
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      errno = 0;
      curr = strtol(buf, &p, 2);
      if (errno) {
        fprintf(stderr, "error parsing %s", buf);
        exit(EXIT_FAILURE);
      }
      lines++;
      for (i = 0; i < 64; ++i)
        counter[i] += (curr >> i) & 1;
    }
  }
  ndigits = 64 - (p - buf);
  for (i = 64; i >= 0; --i)
    gamma = (gamma << 1 | (counter[i] > (lines >> 1)));
  printf("%ld\n", gamma * (~gamma << ndigits >> ndigits));
  return EXIT_SUCCESS;
}
