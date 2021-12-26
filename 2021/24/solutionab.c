#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long
read_const(char buf[32])
{
  char *tok;
  long out;
  if (!(tok = strtok(buf, " "))) {
    fprintf(stderr, "cannot read the first token\n");
    exit(EXIT_FAILURE);
  }
  if (!(tok = strtok(NULL, " "))) {
    fprintf(stderr, "cannot read the second token\n");
    exit(EXIT_FAILURE);
  }
  if (!(tok = strtok(NULL, "\n"))) {
    fprintf(stderr, "cannot read the third token\n");
    exit(EXIT_FAILURE);
  }
  errno = 0;
  out = strtol(tok, NULL, 10);
  if (errno) {
    fprintf(stderr, "cannot parse a number\n");
    exit(EXIT_FAILURE);
  }
  return out;
}

static long
step(long w, long z, size_t i, long A[14], long B[14], long C[14])
{
  if (z % 26 + B[i] == w)
    z /= A[i];
  else
    z = (z / A[i]) * 26 + w + C[i];
  return z;
}

static void
solve(long z, size_t i, long A[14], long B[14], long C[14], long Zmax[14], char digits[15])
{
  char d;
  long znext;
  if (i == 14) {
    if (z == 0) {
      digits[14] = '\0';
      printf("%s\n", digits);
    }
  } else if (z <= Zmax[i]) {
    for (d = 1; d <= 9; ++d) {
      digits[i] = '0' + d;
      znext = step((long) d, z, i, A, B, C);
      solve(znext, i + 1, A, B, C, Zmax, digits);
    }
  }
}

int
main(void)
{
  char buf[32] = {'\0'}, digits[15] = {'\0'};
  size_t block = 0, line = 0, i;
  long A[14], B[14], C[14], Zmax[14];
  while (!feof(stdin)) {
    if (block == 15) {
      fprintf(stderr, "too many blocks\n");
      exit(EXIT_FAILURE);
    }
    if (fgets(buf, sizeof(buf), stdin)) {
      if (strstr(buf, "inp")) {
        block++;
        line = 0;
      }
      if (line == 4)
        A[block - 1] = read_const(buf);
      else if (line == 5)
        B[block - 1] = read_const(buf);
      else if (line == 15)
        C[block - 1] = read_const(buf);
      line++;
    }
  }
  Zmax[13] = 26;
  for (i = 13; i > 1; --i)
    Zmax[i - 1] = Zmax[i] * 26;
  solve(0, 0, A, B, C, Zmax, digits);
  return EXIT_SUCCESS;
}
