#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
  char buf[10] = {'\0'};
  int incs = 0;
  long prev = 0, curr;
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      errno = 0;
      curr = strtol(buf, NULL, 10);
      if (errno) {
        fprintf(stderr, "error parsing %s", buf);
        exit(EXIT_FAILURE);
      }
      incs += curr > prev;
      prev = curr;
    }
  }
  printf("%d\n", incs > 0 ? incs - 1 : incs);
  return EXIT_SUCCESS;
}
