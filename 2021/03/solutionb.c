#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINES 1000

int
main(void)
{
  char buf[32] = {'\0'}, *p;
  int delo2[MAX_LINES] = {0}, delco2[MAX_LINES] = {0};
  int j, majo2, minco2, kepto2, keptco2;
  size_t i, lines = 0;
  long nums[MAX_LINES] = {0}, o2 = 0, co2 = 0, curr, ndigits;
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      errno = 0;
      curr = strtol(buf, &p, 2);
      if (errno) {
        fprintf(stderr, "error parsing %s", buf);
        exit(EXIT_FAILURE);
      }
      nums[lines++] = curr;
    }
  }
  kepto2 = keptco2 = lines;
  ndigits = p - buf;
  for (j = ndigits - 1; j >= 0; --j) {
    o2 = co2 = 0;
    for (i = 0; i < lines; ++i) {
      if (!delo2[i]) o2 += (nums[i] >> j) & 1;
      if (!delco2[i]) co2 += (nums[i] >> j) & 1;
    }
    majo2 = o2 >= kepto2 - o2;
    minco2 = co2 < keptco2 - co2;
    for (i = 0; i < lines; ++i) {
      if (!delo2[i] && kepto2 > 1 && ((nums[i] >> j) & 1) != majo2) {
        kepto2--;
        delo2[i] = 1;
      }
      if (!delco2[i] && keptco2 > 1 && ((nums[i] >> j) & 1) != minco2) {
        keptco2--;
        delco2[i] = 1;
      }
    }
  }
  for (i = 0; i < lines; ++i) {
    if (!delo2[i]) o2 = nums[i];
    if (!delco2[i]) co2 = nums[i];
  }
  printf("%ld\n", o2 * co2);
  return EXIT_SUCCESS;
}
