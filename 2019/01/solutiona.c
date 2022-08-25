#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  char buf[128] = {'\0'};
  long fuel_total = 0l;
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      long mass = strtol(buf, NULL, 10);
      if (errno) {
        fprintf(stderr, "Cannot parse a number\n");
        exit(EXIT_FAILURE);
      }
      fuel_total += floor(mass / 3) - 2;
    }
  }
  printf("%ld\n", fuel_total);
  return EXIT_SUCCESS;
}
