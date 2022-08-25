#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define DIST(x1, y1, x2, y2) (abs((x1) - (x2)) + abs((y1) - (y2)))

int dist(int xcross, int ycross, int *x, int *y, int length)
{
  int result = 0;
  for (int i = 1; i <= length; ++i) {
    if (x[i - 1] == x[i]) { // |
      int y1 = MIN(y[i - 1], y[i]);
      int y2 = MAX(y[i - 1], y[i]);
      if (x[i] == xcross && y1 <= ycross && ycross <= y2) {
        result += DIST(xcross, ycross, x[i - 1], y[i - 1]);
        break;
      } else
        result += DIST(x[i - 1], y[i - 1], x[i], y[i]);
    } else { // -
      int x1 = MIN(x[i - 1], x[i]);
      int x2 = MAX(x[i - 1], x[i]);
      if (x1 <= xcross && xcross <= x2 && y[i] == ycross) {
        result += DIST(xcross, ycross, x[i - 1], y[i - 1]);
        break;
      } else
        result += DIST(x[i - 1], y[i - 1], x[i], y[i]);
    }
  }
  return result;
}

int main(void)
{
  char buf[2048] = {'\0'};
  int x[2][512] = {0}, y[2][512] = {0};
  int edge[2] = {1, 1};
  for (int i = 0; i < 2; ++i) {
    if (!fgets(buf, sizeof(buf), stdin)) {
      fprintf(stderr, "Error reading the line %d\n", i);
      exit(EXIT_SUCCESS);
    }
    for (char *curr = strtok(buf, ","); curr != NULL;
        curr = strtok(NULL, ","), ++edge[i]) {
      int segment = (int) strtol(curr + 1, NULL, 10);
      if (errno) {
        fprintf(stderr, "Error parsing %s\n", curr);
        exit(EXIT_FAILURE);
      }
      switch (curr[0]) {
      case 'R':
        x[i][edge[i]] = x[i][edge[i] - 1] + segment;
        y[i][edge[i]] = y[i][edge[i] - 1];
        break;
      case 'U':
        x[i][edge[i]] = x[i][edge[i] - 1];
        y[i][edge[i]] = y[i][edge[i] - 1] + segment;
        break;
      case 'L':
        x[i][edge[i]] = x[i][edge[i] - 1] - segment;
        y[i][edge[i]] = y[i][edge[i] - 1];
        break;
      case 'D':
        x[i][edge[i]] = x[i][edge[i] - 1];
        y[i][edge[i]] = y[i][edge[i] - 1] - segment;
        break;
      }
    }
  }
  int result = INT_MAX;
  for (int i = 1; i <= edge[0]; ++i) {
    for (int j = 1; j <= edge[1]; ++j) {
      if (x[0][i - 1] == x[0][i]) { // |
        if (x[1][j - 1] == x[1][j]) // -
          continue;
        int x1 = MIN(x[1][j - 1], x[1][j]);
        int x2 = MAX(x[1][j - 1], x[1][j]);
        int y1 = MIN(y[0][i - 1], y[0][i]);
        int y2 = MAX(y[0][i - 1], y[0][i]);
        if ((x1 <= x[0][i] && x[0][i] <= x2) && (y1 <= y[1][j] && y[1][j] <= y2)
            && (x[0][i] != 0 && y[1][j] != 0)) {
          int dist1 = dist(x[0][i], y[1][j], x[0], y[0], edge[0]);
          int dist2 = dist(x[0][i], y[1][j], x[1], y[1], edge[1]);
          result = MIN(result, dist1 + dist2);
        }
      } else { // -
        if (y[1][j - 1] == y[1][j]) // |
          continue;
        int x1 = MIN(x[0][i - 1], x[0][i]);
        int x2 = MAX(x[0][i - 1], x[0][i]);
        int y1 = MIN(y[1][j - 1], y[1][j]);
        int y2 = MAX(y[1][j - 1], y[1][j]);
        if ((x1 <= x[1][j] && x[1][j] <= x2) && (y1 <= y[0][i] && y[0][i] <= y2)
            && (x[1][j] != 0 && y[0][i] != 0)) {
          int dist1 = dist(x[1][j], y[0][i], x[0], y[0], edge[0]);
          int dist2 = dist(x[1][j], y[0][i], x[1], y[1], edge[1]);
          result = MIN(result, dist1 + dist2);
        }
      }
    }
  }
  printf("%d\n", result);
  return EXIT_SUCCESS;
}
