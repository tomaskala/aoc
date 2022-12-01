#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void max3(int max[3], int sum)
{
  if (sum > max[0]) {
    max[2] = max[1];
    max[1] = max[0];
    max[0] = sum;
  } else if (sum > max[1]) {
    max[2] = max[1];
    max[1] = sum;
  } else if (sum > max[2]) {
    max[2] = sum;
  }
}

int main(void)
{
  char buf[1024] = {'\0'};
  int sum = 0, curr;
  int max[3] = {0};

  while (fgets(buf, sizeof(buf), stdin)) {
    if (buf[0] == '\n') {
      max3(max, sum);
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

  max3(max, sum);
  printf("%d\n", max[0] + max[1] + max[2]);
  return EXIT_SUCCESS;
}
