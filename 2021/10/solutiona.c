#include <stdio.h>
#include <stdlib.h>

static int
score(char buf[128], char matches[128], int scores[128])
{
  size_t i, j = 0;
  char stack[128];
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
        return scores[(size_t) buf[i]];
      break;
    }
  }
  return 0;
}

int
main(void)
{
  char buf[128] = {'\0'};
  char matches[128];
  int cost = 0;
  int scores[128];
  matches[')'] = '(';
  matches[']'] = '[';
  matches['}'] = '{';
  matches['>'] = '<';
  scores[')'] = 3;
  scores[']'] = 57;
  scores['}'] = 1197;
  scores['>'] = 25137;
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      cost += score(buf, matches, scores);
    }
  }
  printf("%d\n", cost);
  return EXIT_SUCCESS;
}
