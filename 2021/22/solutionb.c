#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CUBES 420
#define AT(i, j) ((i) * 7 + (j))
#define ROW(i) ((i) * 7)
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

static long
read_num(char *tok)
{
  long num;
  errno = 0;
  num = strtol(tok, NULL, 10);
  if (errno) {
    fprintf(stderr, "cannot parse a number\n");
    exit(EXIT_FAILURE);
  }
  return num;
}

int
main(void)
{
  char buf[64] = {'\0'}, *tok;
  long *cubes = malloc(MAX_CUBES * MAX_CUBES * 7 * sizeof(long));
  long cube1[7], *cube2;
  long volume = 0;
  size_t i, currcubes, ncubes = 0;
  while (!feof(stdin)) {
    while (fgets(buf, sizeof(buf), stdin)) {
      tok = strtok(buf, " ");
      if (!strcmp(tok, "on"))
        cube1[0] = 1;
      else if (!strcmp(tok, "off"))
        cube1[0] = -1;
      else {
        fprintf(stderr, "unexpected first token: %s\n", tok);
        exit(EXIT_FAILURE);
      }
      for (i = 1; i < 4; ++i) {
        tok = strtok(NULL, "=");
        tok = strtok(NULL, "..");
        cube1[2 * i - 1] = read_num(tok);
        tok = strtok(NULL, i < 3 ? "," : "\n");
        if (tok[0] == '.')
          tok++;
        cube1[2 * i] = read_num(tok);
      }
      currcubes = ncubes;
      for (i = 0; i < currcubes; ++i) {
        cube2 = &cubes[ROW(i)];
        cubes[AT(ncubes, 0)] = -cube2[0];
        cubes[AT(ncubes, 1)] = MAX(cube1[1], cube2[1]);
        cubes[AT(ncubes, 2)] = MIN(cube1[2], cube2[2]);
        cubes[AT(ncubes, 3)] = MAX(cube1[3], cube2[3]);
        cubes[AT(ncubes, 4)] = MIN(cube1[4], cube2[4]);
        cubes[AT(ncubes, 5)] = MAX(cube1[5], cube2[5]);
        cubes[AT(ncubes, 6)] = MIN(cube1[6], cube2[6]);
        if (cubes[AT(ncubes, 1)] <= cubes[AT(ncubes, 2)]
            && cubes[AT(ncubes, 3)] <= cubes[AT(ncubes, 4)]
            && cubes[AT(ncubes, 5)] <= cubes[AT(ncubes, 6)])
          ncubes++;
      }
      if (cube1[0] > 0)
        memcpy(&cubes[ROW(ncubes++)], cube1, 7 * sizeof(long));
    }
  }
  for (i = 0; i < ncubes; ++i)
    volume += cubes[AT(i, 0)]
            * (cubes[AT(i, 2)] - cubes[AT(i, 1)] + 1)
            * (cubes[AT(i, 4)] - cubes[AT(i, 3)] + 1)
            * (cubes[AT(i, 6)] - cubes[AT(i, 5)] + 1);
  free(cubes);
  printf("%ld\n", volume);
  return EXIT_SUCCESS;
}
