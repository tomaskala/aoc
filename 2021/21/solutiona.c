#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int
main(void)
{
  char buf[32] = {'\0'};
  int pos[2];
  int score[2] = {0};
  int i, die, rolls;
  size_t player;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "cannot read player 1 position\n");
    exit(EXIT_FAILURE);
  }
  pos[0] = read_position(buf) - 1;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "cannot read player 2 position\n");
    exit(EXIT_FAILURE);
  }
  pos[1] = read_position(buf) - 1;
  for (player = 0, rolls = 0, die = 0;
       score[0] < 1000 && score[1] < 1000;
       player = !player) {
    for (i = 0; i < 3; ++i, ++rolls) {
      die = die % 100 + 1;
      pos[player] += die;
    }
    pos[player] = pos[player] % 10;
    score[player] += pos[player] + 1;
  }
  printf("%d\n", rolls * (score[0] >= 1000 ? score[1] : score[0]));
  return EXIT_SUCCESS;
}
