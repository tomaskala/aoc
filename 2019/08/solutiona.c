#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define W 25
#define H 6
#define D 100

int main(void)
{
  int digit_counts[] = {0, 0, 0};
  int min_zeros = INT_MAX;
  int result = -1;
  for (int k = 0; k < D; ++k) {
    digit_counts[0] = digit_counts[1] = digit_counts[2] = 0;
    for (int i = 0; i < H; ++i) {
      for (int j = 0; j < W; ++j) {
        digit_counts[fgetc(stdin) - '0']++;
      }
    }
    if (digit_counts[0] < min_zeros) {
      min_zeros = digit_counts[0];
      result = digit_counts[1] * digit_counts[2];
    }
  }
  printf("%d\n", result);
  return EXIT_SUCCESS;
}
