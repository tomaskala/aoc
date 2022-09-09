#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))

int signal[] = {0, 1, 2, 3, 4};
int amp;
int phase_set;
int prev_output;

enum instruction {
  ADD = 1,
  MULTIPLY = 2,
  INPUT = 3,
  OUTPUT = 4,
  JUMP_IF_TRUE = 5,
  JUMP_IF_FALSE = 6,
  LESS_THAN = 7,
  EQUALS = 8,
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
  [JUMP_IF_TRUE] = 2,
  [JUMP_IF_FALSE] = 2,
  [LESS_THAN] = 3,
  [EQUALS] = 3,
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
  if (phase_set)
    return prev_output;
  else {
    phase_set = 1;
    return signal[amp];
  }
}

void output(int val)
{
  prev_output = val;
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
    case JUMP_IF_TRUE: {
      int cond = decode(code, ip + 1, mode % 10);
      if (cond != 0) {
        ip = decode(code, ip + 2, (mode / 10) % 10);
        continue;
      }
      break;
    }
    case JUMP_IF_FALSE: {
      int cond = decode(code, ip + 1, mode % 10);
      if (cond == 0) {
        ip = decode(code, ip + 2, (mode / 10) % 10);
        continue;
      }
      break;
    }
    case LESS_THAN:
      BINARY(<);
      break;
    case EQUALS:
      BINARY(==);
      break;
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

int next_permutation(int *arr, size_t len)
{
  int i = len - 1;
  for (; i > 0 && arr[i - 1] >= arr[i]; --i);
  if (i <= 0)
    return 0;
  int j = len - 1;
  for (; arr[j] <= arr[i - 1]; --j);
  int tmp = arr[i - 1];
  arr[i - 1] = arr[j];
  arr[j] = tmp;
  j = len - 1;
  while (i < j) {
    tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
    i++;
    j--;
  }
  return 1;
}

int main(void)
{
  char buf[4096] = {'\0'};
  int code[1024] = {0}, work[1024] = {0};
  int length, result = INT_MIN;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "Error reading input line\n");
    exit(EXIT_FAILURE);
  }
  if ((length = intcode_read(buf, code)) == 0) {
    fprintf(stderr, "Error parsing input\n");
    exit(EXIT_FAILURE);
  }
  do {
    prev_output = 0;
    for (amp = 0; amp < 5; ++amp) {
      for (int i = 0; i < length; ++i)
        work[i] = code[i];
      phase_set = 0;
      intcode_interpret(work, length);
      result = MAX(result, prev_output);
    }
  } while (next_permutation(signal, 5));
  printf("%d\n", result);
  return EXIT_SUCCESS;
}
