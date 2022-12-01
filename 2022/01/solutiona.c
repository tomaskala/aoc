#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  char buf[1024] = {'\0'};
  int sum = 0, max = 0, curr;

  while (fgets(buf, sizeof(buf), stdin)) {
    if (buf[0] == '\n') {
      max = sum > max ? sum : max;
      sum = 0;
    } else {
      curr = (int) strtol(buf, NULL, 10);

      if (errno) {
        fprintf(stderr, "Cannot parse %s\n", buf);
        return EXIT_FAILURE;
      }

      sum += curr;
    }
  }

  max = sum > max ? sum : max;
  printf("%d\n", max);
  return EXIT_SUCCESS;
}
