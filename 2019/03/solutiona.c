#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define DIST(x1, y1) (abs(x1) + abs(y1))

int main(void)
{
  char buf[2048] = {'\0'};
  int x[512] = {0}, y[512] = {0};
  int edge = 1;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "Error reading the first line\n");
    exit(EXIT_SUCCESS);
  }
  for (char *curr = strtok(buf, ","); curr != NULL;
      curr = strtok(NULL, ","), ++edge) {
    int segment = (int) strtol(curr + 1, NULL, 10);
    if (errno) {
      fprintf(stderr, "Error parsing %s\n", curr);
      exit(EXIT_FAILURE);
    }
    switch (curr[0]) {
    case 'R':
      x[edge] = x[edge - 1] + segment;
      y[edge] = y[edge - 1];
      break;
    case 'U':
      x[edge] = x[edge - 1];
      y[edge] = y[edge - 1] + segment;
      break;
    case 'L':
      x[edge] = x[edge - 1] - segment;
      y[edge] = y[edge - 1];
      break;
    case 'D':
      x[edge] = x[edge - 1];
      y[edge] = y[edge - 1] - segment;
      break;
    }
  }
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "Error reading the second line\n");
    exit(EXIT_SUCCESS);
  }
  int xprev = 0, xcurr = 0, yprev = 0, ycurr = 0;
  int result = INT_MAX;
  for (char *curr = strtok(buf, ","); curr != NULL;
      curr = strtok(NULL, ","), ++edge) {
    int segment = (int) strtol(curr + 1, NULL, 10);
    if (errno) {
      fprintf(stderr, "Error parsing %s\n", curr);
      exit(EXIT_FAILURE);
    }
    switch (curr[0]) {
    case 'R':
      xcurr = xprev + segment;
      break;
    case 'U':
      ycurr = yprev + segment;
      break;
    case 'L':
      xcurr = xprev - segment;
      break;
    case 'D':
      ycurr = yprev - segment;
      break;
    }
    for (int i = 1; i <= edge; ++i) {
      if (xprev == xcurr) { // |
        if (x[i - 1] == x[i]) // -
          continue;
        int x1 = MIN(x[i - 1], x[i]);
        int x2 = MAX(x[i - 1], x[i]);
        int y1 = MIN(yprev, ycurr);
        int y2 = MAX(yprev, ycurr);
        if ((x1 <= xcurr && xcurr <= x2) && (y1 <= y[i] && y[i] <= y2)
            && (xcurr != 0 && y[i] != 0))
          result = MIN(result, DIST(xcurr, y[i]));
      } else { // -
        if (y[i - 1] == y[i]) // |
          continue;
        int x1 = MIN(xprev, xcurr);
        int x2 = MAX(xprev, xcurr);
        int y1 = MIN(y[i - 1], y[i]);
        int y2 = MAX(y[i - 1], y[i]);
        if ((x1 <= x[i] && x[i] <= x2) && (y1 <= ycurr && ycurr <= y2)
            && (x[i] != 0 && ycurr != 0))
          result = MIN(result, DIST(x[i], ycurr));
      }
    }
    xprev = xcurr;
    yprev = ycurr;
  }
  printf("%d\n", result);
  return EXIT_SUCCESS;
}
