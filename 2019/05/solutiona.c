#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

enum instruction {
  ADD = 1,
  MULTIPLY = 2,
  INPUT = 3,
  OUTPUT = 4,
  HALT = 99,
};

enum parameter_mode {
  POSITION = 0,
  IMMEDIATE = 1,
};

int arity[] = {
  [ADD] = 3,
  [MULTIPLY] = 3,
  [INPUT] = 1,
  [OUTPUT] = 1,
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

int decode(int *code, int ip, enum parameter_mode mode)
{
  switch (mode) {
  case POSITION:
    return code[code[ip]];
  case IMMEDIATE:
    return code[ip];
  default:
    fprintf(stderr, "Unexpected parameter mode '%d'\n", mode);
    exit(EXIT_FAILURE);
  }
}

int input()
{
  return 1;
}

void output(int val)
{
  printf("%d\n", val);
}

void intcode_interpret(int *code, int length)
{
  #define BINARY(op) \
    do { \
      int a = decode(code, ip + 1, mode % 10); \
      int b = decode(code, ip + 2, (mode / 10) % 10); \
      CHECK_WRITE_MODE(mode / 100); \
      STORE(ip + 3, a op b); \
    } while (0)
  #define STORE(i, val) (code[code[i]] = val)
  #define CHECK_WRITE_MODE(mode) \
    do { \
      if (mode != 0) { \
        fprintf(stderr, \
            "The write parameter for opcode %d must be in mode 0, got '%d'\n", \
            opcode, mode); \
        exit(EXIT_FAILURE); \
      } \
    } while (0)
  int ip = 0;
  while (ip < length) {
    int mode = code[ip] / 100;
    int opcode = code[ip] % 100;
    switch (opcode) {
    case ADD: {
      BINARY(+);
      break;
    }
    case MULTIPLY: {
      BINARY(*);
      break;
    }
    case INPUT:
      CHECK_WRITE_MODE(mode);
      int val = input();
      STORE(ip + 1, val);
      break;
    case OUTPUT: {
      int val = decode(code, ip + 1, mode);
      output(val);
      break;
    }
    case HALT:
      return;
    default:
      fprintf(stderr, "Unexpected instruction '%d' at position '%d'\n",
          opcode, ip);
      exit(EXIT_FAILURE);
    }
    ip += arity[opcode] + 1;
  }
  #undef CHECK_WRITE_MODE
  #undef STORE
  #undef BINARY
}

int main(void)
{
  char buf[4096] = {'\0'};
  int code[1024] = {0};
  int length;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "Error reading input line\n");
    exit(EXIT_FAILURE);
  }
  if ((length = intcode_read(buf, code)) == 0) {
    fprintf(stderr, "Error parsing input\n");
    exit(EXIT_FAILURE);
  }
  intcode_interpret(code, length);
  return EXIT_SUCCESS;
}
