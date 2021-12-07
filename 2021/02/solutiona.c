#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(void)
{
  char buf[32] = {'\0'}, *tok, op;
  long curr, h = 0, v = 0;
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      if (!(tok = strtok(buf, " "))) {
        fprintf(stderr, "invalid token1\n");
        exit(EXIT_FAILURE);
      }
      op = tok[0];
      if (!(tok = strtok(NULL, " "))) {
        fprintf(stderr, "invalid token2\n");
        exit(EXIT_FAILURE);
      }
      errno = 0;
      curr = strtol(tok, NULL, 10);
      if (errno) {
        fprintf(stderr, "error parsing %s", tok);
        exit(EXIT_FAILURE);
      }
      switch (op) {
      case 'f':
        h += curr;
        break;
      case 'd':
        v += curr;
        break;
      case 'u':
        v -= curr;
        break;
      default:
        fprintf(stderr, "unexpected input %s", buf);
        exit(EXIT_FAILURE);
      }
    }
  }
  printf("%ld\n", h * v);
  return EXIT_SUCCESS;
}
