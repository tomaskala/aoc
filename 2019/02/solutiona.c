#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int arity[] = {
  [1] = 3,
  [2] = 3,
  [99] = 0,
};

int intcode_read(char *buf, int *out)
{
  int i;
  char *endptr = buf;
  for (i = 0; *endptr != '\0' && *endptr != '\n'; ++i) {
    out[i] = (int) strtol(buf, &endptr, 10);
    if (errno)
      return 0;
    while (*endptr != '\0' && (*endptr < '0' || '9' < *endptr))
      endptr++;
    buf = endptr;
  }
  return i;
}

int intcode_interpret(int *code, int length)
{
  #define BINARY(op) (code[code[ip + 3]] = \
      code[code[ip + 1]] op code[code[ip + 2]])
  int ip = 0;
  while (ip < length) {
    switch (code[ip]) {
    case 1:
      BINARY(+);
      break;
    case 2:
      BINARY(*);
      break;
    }
    if (code[ip] == 99)
      break;
    ip += arity[code[ip]] + 1;
  }
  #undef BINARY
  return code[0];
}

int main(void)
{
  char buf[512] = {'\0'};
  int code[256] = {0};
  int length;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "Error reading input line\n");
    exit(EXIT_FAILURE);
  }
  if ((length = intcode_read(buf, code)) == 0) {
    fprintf(stderr, "Error parsing input\n");
    exit(EXIT_FAILURE);
  }
  code[1] = 12;
  code[2] = 2;
  int result = intcode_interpret(code, length);
  printf("%d\n", result);
  return EXIT_SUCCESS;
}
