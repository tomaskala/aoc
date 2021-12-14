#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STEPS 10
#define MAXLEN 64
#define N 128
#define AT(i, j) ((i) * (N) + (j))

static void
step(char rules[N * N], long elements[N], long pairs[N * N], long work[N * N])
{
  size_t lhs1, lhs2, rhs;
  for (lhs1 = 0; lhs1 < N; ++lhs1) {
    for (lhs2 = 0; lhs2 < N; ++lhs2) {
      if (!rules[AT(lhs1, lhs2)])
        continue;
      rhs = rules[AT(lhs1, lhs2)];
      elements[rhs] += pairs[AT(lhs1, lhs2)];
      work[AT(lhs1, rhs)] += pairs[AT(lhs1, lhs2)];
      work[AT(rhs, lhs2)] += pairs[AT(lhs1, lhs2)];
    }
  }
}

int
main(void)
{
  char buf[MAXLEN] = {'\0'}, seq[MAXLEN] = {'\0'}, *tok1, *tok2;
  char rules[N * N] = {'\0'};
  int part = 0;
  long elements[N] = {0}, pairs[N * N] = {0}, work[N * N];
  long min = LONG_MAX;
  long max = LONG_MIN;
  size_t i, seqlen = 0;
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      if (buf[0] == '\n') {
        part = 1;
        continue;
      }
      if (!part) {
        seqlen = strlen(buf);
        if (buf[seqlen - 1] == '\n')
          buf[--seqlen] = '\0';
        memcpy(seq, buf, seqlen);
      } else {
        if (!(tok1 = strtok(buf, " -> "))) {
          fprintf(stderr, "cannot parse the first token\n");
          exit(EXIT_FAILURE);
        }
        if (strlen(tok1) != 2) {
          fprintf(stderr, "the first token must consist of 2 letters\n");
          exit(EXIT_FAILURE);
        }
        if (!(tok2 = strtok(NULL, " -> "))) {
          fprintf(stderr, "cannot parse the second token\n");
          exit(EXIT_FAILURE);
        }
        if (strlen(tok2) == 2 && tok2[1] == '\n')
          tok2[1] = '\0';
        if (strlen(tok2) != 1) {
          fprintf(stderr, "the second token must consist of 1 letter\n");
          exit(EXIT_FAILURE);
        }
        rules[AT(tok1[0], tok1[1])] = tok2[0];
      }
    }
  }
  for (i = 0; i < seqlen; ++i) {
    elements[(size_t) seq[i]]++;
    if (i < seqlen - 1)
      pairs[AT((size_t) seq[i], (size_t) seq[i + 1])]++;
  }
  for (i = 0, part = 0; i < STEPS; ++i, part = !part) {
    if (!part) {
      memset(work, 0x00, sizeof(work));
      step(rules, elements, pairs, work);
    } else {
      memset(pairs, 0x00, sizeof(pairs));
      step(rules, elements, work, pairs);
    }
  }
  for (i = 0; i < N; ++i) {
    if (elements[i] > 0 && elements[i] > max)
      max = elements[i];
    if (elements[i] > 0 && elements[i] < min)
      min = elements[i];
  }
  printf("%ld\n", max - min);
  return EXIT_SUCCESS;
}
