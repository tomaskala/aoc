#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  long ymin, v0, res;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "cannot read the input\n");
    exit(EXIT_FAILURE);
  }
  tok = strtok(buf, "=");
  tok = strtok(NULL, "=");
  tok = strtok(NULL, ".");
  ymin = read_number(tok);
  v0 = -ymin - 1;
  res = v0 * (v0 + 1) / 2;
  printf("%ld\n", res);
  return EXIT_SUCCESS;
}
