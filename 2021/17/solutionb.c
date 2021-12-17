#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIGN(x) ((x) < 0 ? -1 : (x) > 0 ? 1 : 0)

static long
read_number(char *tok)
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
  long state[4] = {0};
  long xmin, xmax, ymin, ymax, vx, vy, res = 0;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "cannot read the input\n");
    exit(EXIT_FAILURE);
  }
  tok = strtok(buf, "=");
  tok = strtok(NULL, ".");
  xmin = read_number(tok);
  tok = strtok(NULL, ",");
  xmax = read_number(tok + 1);
  tok = strtok(NULL, "=");
  tok = strtok(NULL, ".");
  ymin = read_number(tok);
  tok = strtok(NULL, "\n");
  ymax = read_number(tok + 1);
  for (vx = 0; vx <= xmax; ++vx) {
    for (vy = ymin; vy <= -ymin; ++vy) {
      state[0] = 0;
      state[1] = 0;
      state[2] = vx;
      state[3] = vy;
      while (state[0] <= xmax && ymin <= state[1]) {
        if (xmin <= state[0] && state[1] <= ymax) {
          res++;
          break;
        }
        state[0] += state[2];
        state[1] += state[3];
        state[2] -= SIGN(state[2]);
        state[3] -= 1;
      }
    }
  }
  printf("%ld\n", res);
  return EXIT_SUCCESS;
}
