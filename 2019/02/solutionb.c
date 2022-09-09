#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

enum instruction {
  ADD = 1,
  MULTIPLY = 2,
  HALT = 99,
};

int arity[] = {
  [ADD] = 3,
  [MULTIPLY] = 3,
  [HALT] = 0,
};

int intcode_read(char *buf, int *out)
{
  int i;
  char *endptr = buf;
  for (i = 0; *endptr != '\0' && *endptr != '\n'; ++i) {
    out[i] = (int) strtol(buf, &endptr, 10);
    if (errno)
      return 0;
    buf = endptr + 1;
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
    case ADD:
      BINARY(+);
      break;
    case MULTIPLY:
      BINARY(*);
      break;
    case HALT:
      goto done;
    default:
      fprintf(stderr, "Unexpected instruction '%d' at position '%d'\n",
          code[ip], ip);
      exit(EXIT_FAILURE);
    }
    ip += arity[code[ip]] + 1;
  }
done:
  #undef BINARY
  return code[0];
}

int main(void)
{
  char buf[512] = {'\0'};
  int code[256] = {0}, work[256] = {0};
  int length;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "Error reading input line\n");
    exit(EXIT_FAILURE);
  }
  if ((length = intcode_read(buf, code)) == 0) {
    fprintf(stderr, "Error parsing input\n");
    exit(EXIT_FAILURE);
  }
  int noun, verb;
  for (noun = 0; noun < 100; ++noun) {
    for (verb = 0; verb < 100; ++verb) {
      for (int i = 0; i < length; ++i)
        work[i] = code[i];
      work[1] = noun;
      work[2] = verb;
      int result = intcode_interpret(work, length);
      if (result == 19690720)
        goto done;
    }
  }
done:
  printf("%d\n", 100 * noun + verb);
  return EXIT_SUCCESS;
}