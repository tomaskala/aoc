#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SCORE 21

static int rolls[7] = {3, 4, 5, 6, 7, 8, 9};
static int counts[10] = {0, 0, 0, 1, 3, 6, 7, 6, 3, 1};

struct state {
  long wins0;
  long wins1;
};

static int
read_position(char buf[32])
{
  char *tok;
  size_t num;
  if (!(tok = strtok(buf, ":"))) {
    fprintf(stderr, "cannot tokenize the input\n");
    exit(EXIT_FAILURE);
  }
  if (!(tok = strtok(NULL, "\n"))) {
    fprintf(stderr, "cannot read the position token\n");
    exit(EXIT_FAILURE);
  }
  errno = 0;
  num = strtol(tok, NULL, 10);
  if (errno) {
    fprintf(stderr, "cannot read a number\n");
    exit(EXIT_FAILURE);
  }
  return (int) num;
}

static struct state
turn(int player, int score0, int score1, int pos0, int pos1)
{
  size_t i;
  int newscore0 = score0, newscore1 = score1, newpos0 = pos0, newpos1 = pos1;
  struct state s1, s2;
  if (score0 >= MAX_SCORE) {
    s1.wins0 = 1;
    s1.wins1 = 0;
    return s1;
  } else if (score1 >= MAX_SCORE) {
    s1.wins0 = 0;
    s1.wins1 = 1;
    return s1;
  } else {
    s1.wins0 = 0;
    s1.wins1 = 0;
    for (i = 0; i < 7; ++i) {
      if (player) {
        newpos1 = (pos1 + rolls[i]) % 10;
        newscore1 = score1 + newpos1 + 1;
      } else {
        newpos0 = (pos0 + rolls[i]) % 10;
        newscore0 = score0 + newpos0 + 1;
      }
      s2 = turn(!player, newscore0, newscore1, newpos0, newpos1);
      s1.wins0 += s2.wins0 * counts[rolls[i]];
      s1.wins1 += s2.wins1 * counts[rolls[i]];
    }
    return s1;
  }
}

int
main(void)
{
  char buf[32] = {'\0'};
  int pos0, pos1;
  struct state s;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "cannot read player 1 position\n");
    exit(EXIT_FAILURE);
  }
  pos0 = read_position(buf) - 1;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "cannot read player 2 position\n");
    exit(EXIT_FAILURE);
  }
  pos1 = read_position(buf) - 1;
  s = turn(0, 0, 0, pos0, pos1);
  printf("%ld\n", s.wins0 > s.wins1 ? s.wins0 : s.wins1);
  return EXIT_SUCCESS;
}
