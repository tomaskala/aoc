#include <stdio.h>
#include <stdlib.h>

#define MAX_LINES 128

static int
cmp(const void *a, const void *b)
{
  const long *ia = (long *) a;
  const long *ib = (long *) b;
  return (*ia > *ib) - (*ia < *ib);
}

static long
score(char buf[MAX_LINES], char matches[MAX_LINES], int scores[MAX_LINES])
{
  size_t i, j = 0;
  long cost = 0;
  char stack[MAX_LINES];
  for (i = 0; buf[i] != '\n'; ++i) {
    switch (buf[i]) {
    case '(':
    case '[':
    case '{':
    case '<':
      stack[j++] = buf[i];
      break;
    case ')':
    case ']':
    case '}':
    case '>':
      if (stack[--j] != matches[(size_t) buf[i]])
        return 0;
      break;
    }
  }
  for (i = 0; i < j; ++i) {
    cost *= 5;
    cost += scores[(size_t) matches[(size_t) stack[j - i - 1]]];
  }
  return cost;
}

int
main(void)
{
  char buf[MAX_LINES] = {'\0'};
  char matches[MAX_LINES];
  size_t total = 0;
  int scores[MAX_LINES];
  long cost = 0;
  long costs[MAX_LINES];
  matches['('] = ')';
  matches['['] = ']';
  matches['{'] = '}';
  matches['<'] = '>';
  matches[')'] = '(';
  matches[']'] = '[';
  matches['}'] = '{';
  matches['>'] = '<';
  scores[')'] = 1;
  scores[']'] = 2;
  scores['}'] = 3;
  scores['>'] = 4;
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      if (total == MAX_LINES) {
        fprintf(stderr, "too many lines in the input\n");
        exit(EXIT_FAILURE);
      }
      cost = score(buf, matches, scores);
      if (cost > 0)
        costs[total++] = cost;
    }
  }
  qsort(costs, total, sizeof(long), &cmp);
  printf("%ld\n", costs[total / 2]);
  return EXIT_SUCCESS;
}
