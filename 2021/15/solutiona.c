#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MMAX 100
#define NMAX 100
#define AT(i, j) ((i) * (NMAX) + (j))

struct elem {
  size_t i;
  size_t j;
  long priority;
};

struct heap {
  size_t size;
  struct elem *elems[MMAX * NMAX + 1];
};

static struct elem *
elem_new(size_t i, size_t j, long priority)
{
  struct elem *e = malloc(sizeof(struct elem));
  e->i = i;
  e->j = j;
  e->priority = priority;
  return e;
}

static void
elem_free(struct elem *e)
{
  free(e);
}

static struct heap *
heap_new()
{
  struct heap *h = malloc(sizeof(struct heap));
  h->size = 0;
  h->elems[0] = NULL;
  return h;
}

static void
heap_push(struct heap *h, struct elem *e)
{
  int found = 0;
  size_t i = h->size + 1, parent;
  struct elem *tmp;
  h->elems[i] = e;
  while (!found && i > 1) {
    parent = i / 2;
    if (h->elems[i]->priority < h->elems[parent]->priority) {
      tmp = h->elems[i];
      h->elems[i] = h->elems[i / 2];
      h->elems[i / 2] = tmp;
      i /= 2;
    } else
      found = 1;
  }
  h->size++;
}

static struct elem *
heap_pop(struct heap *h)
{
  int found = 0;
  size_t i = 1, left, right, child;
  struct elem *e = h->elems[1], *tmp;
  h->elems[1] = h->elems[h->size--];
  while (!found && 2 * i <= h->size) {
    left = 2 * i;
    right = 2 * i + 1;
    child = left;
    if (2 * i + 1 <= h->size
        && h->elems[right]->priority < h->elems[left]->priority)
      child = right;
    if (h->elems[i]->priority > h->elems[child]->priority) {
      tmp = h->elems[i];
      h->elems[i] = h->elems[child];
      h->elems[child] = tmp;
      i = child;
    } else
      found = 1;
  }
  return e;
}

static void
heap_free(struct heap *h)
{
  free(h);
}

static long
dijkstra(long matrix[MMAX * NMAX], size_t m, size_t n)
{
  #define PUSH(vi, vj) do { \
    if (!visited[AT(vi, vj)] && dist[AT(e->i, e->j)] + matrix[AT(vi, vj)] \
        < dist[AT(vi, vj)]) { \
      dist[AT(vi, vj)] = dist[AT(e->i, e->j)] + matrix[AT(vi, vj)]; \
      f = elem_new(vi, vj, dist[AT(vi, vj)]); \
      heap_push(h, f); \
    } \
  } while (0)
  int visited[MMAX * NMAX] = {0};
  long dist[MMAX * NMAX] = {0};
  size_t i;
  struct elem *e = elem_new(0, 0, matrix[0]), *f;
  struct heap *h = heap_new();
  for (i = 1; i < MMAX * NMAX; ++i)
    dist[i] = LONG_MAX;
  heap_push(h, e);
  while (h->size > 0) {
    e = heap_pop(h);
    visited[AT(e->i, e->j)] = 1;
    if (e->i > 0)
      PUSH(e->i - 1, e->j);
    if (e->j > 0)
      PUSH(e->i, e->j - 1);
    if (e->i < m - 1)
      PUSH(e->i + 1, e->j);
    if (e->j < n - 1)
      PUSH(e->i, e->j + 1);
    elem_free(e);
  }
  heap_free(h);
  return dist[AT(m - 1, n - 1)];
}

int
main(void)
{
  char buf[128] = {'\0'};
  size_t i, j, m, n = 0;
  long matrix[MMAX * NMAX] = {0}, risk;
  for (i = 0, m = 0; fgets(buf, sizeof(buf), stdin); ++i, ++m) {
    if (i == MMAX) {
      fprintf(stderr, "too many rows\n");
      exit(EXIT_FAILURE);
    }
    for (j = 0, n = 0; buf[j] != '\n' && buf[j] != '\0'; ++j, ++n) {
      if (j == NMAX) {
        fprintf(stderr, "too many columns\n");
        exit(EXIT_FAILURE);
      }
      if (buf[j] < '0' || '9' < buf[j]) {
        fprintf(stderr, "cannot parse a number\n");
        exit(EXIT_FAILURE);
      }
      matrix[AT(i, j)] = buf[j] - '0';
    }
  }
  risk = dijkstra(matrix, m, n);
  printf("%ld\n", risk);
  return EXIT_SUCCESS;
}
