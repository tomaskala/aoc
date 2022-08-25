#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int digits_decreasing(int n)
{
  int d1 = n % 10, d2;
  n /= 10;
  for (int i = 0; i < 5; ++i) {
    d2 = n % 10;
    if (d1 < d2)
      return 1;
    d1 = d2;
    n /= 10;
  }
  return 0;
}

int same_adjacent_digits(int n)
{
  int d1 = n % 10, d2;
  n /= 10;
  for (int i = 0; i < 5; ++i) {
    d2 = n % 10;
    if (d1 == d2)
      return 1;
    d1 = d2;
    n /= 10;
  }
  return 0;
}

int main(void)
{
  char buf[64] = {'\0'};
  char *endptr;
  int left, right;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "Cannot parse the input\n");
    exit(EXIT_FAILURE);
  }
  left = strtol(buf, &endptr, 10);
  if (errno) {
    fprintf(stderr, "Error parsing a number\n");
    exit(EXIT_FAILURE);
  }
  right = strtol(endptr + 1, NULL, 10);
  if (errno) {
    fprintf(stderr, "Error parsing a number\n");
    exit(EXIT_FAILURE);
  }
  int valid_passwords = 0;
  for (int i = left; i <= right; ++i) {
    if (digits_decreasing(i))
      continue;
    if (!same_adjacent_digits(i))
      continue;
    valid_passwords++;
  }
  printf("%d\n", valid_passwords);
  return EXIT_SUCCESS;
}
