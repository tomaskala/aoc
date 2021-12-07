#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
  char buf[10] = {'\0'};
  int incs = 0, lines = 0;
  size_t r = 0, w = 0;
  long curr, prevsum = 0, currsum = 0;
  long cbuf[3] = {0};
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      errno = 0;
      curr = strtol(buf, NULL, 10);
      if (errno) {
        fprintf(stderr, "error parsing %s", buf);
        exit(EXIT_FAILURE);
      }
      currsum = prevsum + curr - cbuf[r];
      r = (r + 1) % 3;
      cbuf[w] = curr;
      w = (w + 1) % 3;
      incs += currsum > prevsum;
      prevsum = currsum;
      lines++;
    }
  }
  printf("%d\n", lines > 3 ? incs - 3 : 0);
  return EXIT_SUCCESS;
}
