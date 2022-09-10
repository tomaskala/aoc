#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define W 25
#define H 6
#define D 100

int main(void)
{
  int image[D][H][W] = {0};
  for (int k = 0; k < D; ++k)
    for (int i = 0; i < H; ++i)
      for (int j = 0; j < W; ++j)
        image[k][i][j] = fgetc(stdin) - '0';
  for (int i = 0; i < H; ++i) {
    for (int j = 0; j < W; ++j) {
      for (int k = 0; k < D; ++k) {
        if (image[k][i][j] != 2) {
          image[0][i][j] = image[k][i][j];
          break;
        }
      }
    }
  }
  for (int i = 0; i < H; ++i) {
    for (int j = 0; j < W; ++j)
      printf("%c", image[0][i][j] == 1 ? '*' : ' ');
    printf("\n");
  }
  return EXIT_SUCCESS;
}
