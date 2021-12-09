#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define A (segments[0])
#define B (segments[1])
#define C (segments[2])
#define D (segments[3])
#define E (segments[4])
#define F (segments[5])
#define G (segments[6])

int
main(void)
{
  char buf[128], *tok1, *tok2, *tok3;
  int result = 0, display, bitrep;
  int bitmask[7] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};
  int orders[4] = {1000, 100, 10, 1};
  int bits[7], digits[128], segments[7];
  size_t i, j, len;
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      tok1 = strtok(buf, "|");
      tok2 = strtok(NULL, "\n");
      for (i = 0; i < 7; ++i)
        bits[i] = 0x7F;
      for (tok3 = strtok(tok1, " "); tok3; tok3 = strtok(NULL, " ")) {
        bitrep = 0x00;
        for (j = 0, len = 0; tok3[j]; ++j, ++len)
          bitrep |= bitmask[tok3[j] - 'a'];
        bits[len] &= bitrep;
      }
      A = 0x7F & ~bits[2] &  bits[3];
      B = 0x7F & ~bits[3] &  bits[4] & bits[6];
      C = 0x7F &  bits[2] & ~bits[6];
      D = 0x7F &  bits[4] &  bits[5];
      E = 0x7F & ~bits[4] & ~bits[6];
      F = 0x7F &  bits[2] &  bits[6];
      G = 0x7F & ~bits[3] &  bits[5] & bits[6];
      digits[(A | B | C |     E | F | G)] = 0;
      digits[(        C |         F    )] = 1;
      digits[(A |     C | D | E |     G)] = 2;
      digits[(A |     C | D |     F | G)] = 3;
      digits[(    B | C | D |     F    )] = 4;
      digits[(A | B |     D |     F | G)] = 5;
      digits[(A | B |     D | E | F | G)] = 6;
      digits[(A |     C |         F    )] = 7;
      digits[(A | B | C | D | E | F | G)] = 8;
      digits[(A | B | C | D |     F | G)] = 9;
      display = 0;
      for (i = 0, tok3 = strtok(tok2, " ");
           i < 4 && tok3;
           ++i, tok3 = strtok(NULL, " ")) {
        bitrep = 0x00;
        for (j = 0; tok3[j]; ++j)
          bitrep |= bitmask[tok3[j] - 'a'];
        display += digits[bitrep] * orders[i];
      }
      result += display;
    }
  }
  printf("%d\n", result);
  return EXIT_SUCCESS;
}
