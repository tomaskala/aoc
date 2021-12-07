#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMS 1000
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define SWAP(arr, i, j) do { \
  long tmp = arr[i]; \
  arr[i] = arr[j]; \
  arr[j] = tmp; \
  } while (0)

static long
cost(long pos, long positions[MAX_NUMS], size_t total)
{
  size_t i;
  long result = 0;
  for (i = 0; i < total; ++i)
    result += ABS(positions[i] - pos);
  return result;
}

static size_t
partition(long positions[MAX_NUMS], size_t left, size_t right, size_t pivot)
{
  size_t i, store = left;
  long pivotval = positions[pivot];
  SWAP(positions, pivot, right);
  for (i = left; i < right - 1; ++i)
    if (positions[i] < pivotval) {
      SWAP(positions, i, store);
      store++;
    }
  SWAP(positions, right, store);
  return store;
}

static long
quickselect(long positions[MAX_NUMS], size_t left, size_t right, size_t k)
{
  size_t pivot;
  while (1) {
    if (left == right)
      return positions[left];
    pivot = partition(positions, left, right, left);
    if (pivot == k)
      return positions[k];
    else if (k < pivot)
      right = pivot - 1;
    else
      left = pivot + 1;
  }
}

int
main(void)
{
  char buf[4000] = {'\0'}, *tok;
  size_t total = 0;
  long positions[MAX_NUMS] = {0}, curr, result;
  if (!fgets(buf, sizeof(buf), stdin)) {
    fprintf(stderr, "cannot read the input\n");
    exit(EXIT_FAILURE);
  }
  while ((tok = strtok(total ? NULL : buf, ","))) {
    errno = 0;
    curr = strtol(tok, NULL, 10);
    if (errno) {
      fprintf(stderr, "cannot parse a number\n");
      exit(EXIT_FAILURE);
    }
    if (total == MAX_NUMS) {
      fprintf(stderr, "%lu the input is too large\n", total);
      exit(EXIT_FAILURE);
    }
    positions[total++] = curr;
  }
  if (total % 2 == 0) {
    curr = quickselect(positions, 0, total - 1, total / 2);
    result = cost(curr, positions, total);
    curr = quickselect(positions, 0, total - 1, total / 2 - 1);
    result = MIN(result, cost(curr, positions, total));
  } else {
    curr = quickselect(positions, 0, total - 1, total / 2);
    result = cost(curr, positions, total);
  }
  printf("%ld\n", result);
  return EXIT_SUCCESS;
}
