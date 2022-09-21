#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define VALUE_FMT "%lld"
#define NEXT_CAPACITY(capacity) ((capacity) < 8 ? 8 : 2 * (capacity))

typedef long long Value;

struct code {
  Value *chunk;
  int length;
  int capacity;
};

struct vm {
  struct code *code;
  int ip;
  int base;
};

enum instruction {
  ADD = 1,
  MULTIPLY = 2,
  INPUT = 3,
  OUTPUT = 4,
  JUMP_IF_TRUE = 5,
  JUMP_IF_FALSE = 6,
  LESS_THAN = 7,
  EQUALS = 8,
  ADJUST_BASE = 9,
  HALT = 99,
};

enum parameter_mode {
  POSITION = 0,
  IMMEDIATE = 1,
  RELATIVE = 2,
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
  [ADJUST_BASE] = 1,
  [HALT] = 0,
};

Value max(Value x, Value y)
{
  return x > y ? x : y;
}

void *reallocate(void *ptr, size_t new_size)
{
  if (new_size == 0) {
    free(ptr);
    return NULL;
  }
  void *result = realloc(ptr, new_size);
  if (result == NULL)
    exit(EXIT_FAILURE);
  return result;
}

void code_init(struct code *code)
{
  code->chunk = NULL;
  code->length = 0;
  code->capacity = 0;
}

void code_grow(struct code *code, int new_capacity) {
  code->chunk = reallocate(code->chunk, new_capacity * sizeof(Value));
  for (int i = code->capacity; i < new_capacity; ++i)
    code->chunk[i] = 0;
  code->capacity = new_capacity;
}

void code_free(struct code *code)
{
  code->chunk = reallocate(code->chunk, 0);
  code_init(code);
}

Value code_get(struct code *code, int i)
{
  if (i >= code->capacity) {
    int new_capacity = max(i + 1, NEXT_CAPACITY(code->capacity));
    code_grow(code, new_capacity);
  }
  return code->chunk[i];
}

void code_append(struct code *code, Value elem)
{
  if (code->capacity < code->length + 1) {
    int new_capacity = NEXT_CAPACITY(code->capacity);
    code_grow(code, new_capacity);
  }
  code->chunk[code->length++] = elem;
}

void code_set(struct code *code, int i, Value elem)
{
  if (i >= code->capacity) {
    int new_capacity = max(i + 1, NEXT_CAPACITY(code->capacity));
    code_grow(code, new_capacity);
  }
  code->chunk[i] = elem;
}

void vm_init(struct vm *vm, struct code *code)
{
  vm->code = code;
  vm->ip = 0;
  vm->base = 0;
}

void vm_free(struct vm *vm)
{
  code_free(vm->code);
  vm_init(vm, NULL);
}

void intcode_read(char *buf, struct code *code)
{
  char *endptr = buf;
  for (int i = 0; *endptr != '\0' && *endptr != '\n'; ++i) {
    Value elem = (Value) strtol(buf, &endptr, 10);
    if (errno)
      return;
    buf = endptr + 1;
    code_append(code, elem);
  }
}

int decode(struct vm *vm, int offset, enum parameter_mode mode)
{
  switch (mode) {
  case POSITION:
    return (int) code_get(vm->code, vm->ip + offset);
  case IMMEDIATE:
    return vm->ip + offset;
  case RELATIVE:
    return vm->base + code_get(vm->code, vm->ip + offset);
  default:
    fprintf(stderr, "Unexpected parameter mode '%d'\n", mode);
    vm_free(vm);
    exit(EXIT_FAILURE);
  }
}

Value input()
{
  return (Value) 2;
}

void output(Value val)
{
  printf(VALUE_FMT "\n", val);
}

void intcode_interpret(struct vm *vm)
{
  #define BINARY(op) \
    do { \
      Value a = code_get(vm->code, decode(vm, 1, mode % 10)); \
      Value b = code_get(vm->code, decode(vm, 2, (mode / 10) % 10)); \
      int i = decode(vm, 3, (mode / 100) % 10); \
      code_set(vm->code, i, a op b); \
    } while (0)
  while (1) {
    Value codepoint = code_get(vm->code, vm->ip);
    Value mode = codepoint / 100;
    Value opcode = codepoint % 100;
    switch (opcode) {
    case ADD: {
      BINARY(+);
      break;
    }
    case MULTIPLY: {
      BINARY(*);
      break;
    }
    case INPUT: {
      Value val = input();
      int i = decode(vm, 1, mode % 10);
      code_set(vm->code, i, val);
      break;
    }
    case OUTPUT: {
      Value val = code_get(vm->code, decode(vm, 1, mode));
      output(val);
      break;
    }
    case JUMP_IF_TRUE: {
      Value cond = code_get(vm->code, decode(vm, 1, mode % 10));
      if (cond != (Value) 0) {
        vm->ip = (int) code_get(vm->code, decode(vm, 2, (mode / 10) % 10));
        continue;
      }
      break;
    }
    case JUMP_IF_FALSE: {
      Value cond = code_get(vm->code, decode(vm, 1, mode % 10));
      if (cond == (Value) 0) {
        vm->ip = (int) code_get(vm->code, decode(vm, 2, (mode / 10) % 10));
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
    case ADJUST_BASE: {
      int val = (int) code_get(vm->code, decode(vm, 1, mode));
      vm->base += val;
      break;
    }
    case HALT:
      return;
    default:
      fprintf(stderr, "Unexpected instruction '" VALUE_FMT "' " \
          "at position '%d'\n", opcode, vm->ip);
      vm_free(vm);
      exit(EXIT_FAILURE);
    }
    vm->ip += arity[opcode] + 1;
  }
  #undef BINARY
}

int main(void)
{
  char buf[4096] = {'\0'};
  struct code code;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "Error reading input line\n");
    exit(EXIT_FAILURE);
  }
  code_init(&code);
  intcode_read(buf, &code);
  if (code.length == 0) {
    fprintf(stderr, "Error parsing input\n");
    code_free(&code);
    exit(EXIT_FAILURE);
  }
  struct vm vm;
  vm_init(&vm, &code);
  intcode_interpret(&vm);
  vm_free(&vm);
  return EXIT_SUCCESS;
}
