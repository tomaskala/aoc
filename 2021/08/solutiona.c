#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(void)
{
  char buf[128], *tok1, *tok2;
  int total = 0;
  size_t i, len;
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      i = 0;
      tok1 = strtok(buf, "|");
      tok1 = strtok(NULL, "\n");
      while ((tok2 = strtok(i ? NULL : tok1, " "))) {
        i++;
        len = strlen(tok2);
        total += len == 2 || len == 3 || len == 4 || len == 7;
      }
    }
  }
  printf("%d\n", total);
  return EXIT_SUCCESS;
}
