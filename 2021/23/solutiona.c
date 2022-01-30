#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define AMPHIPODS_PER_ROOM 2
#define N_ROOMS 4
#define N_AMPHIPODS (AMPHIPODS_PER_ROOM * N_ROOMS)
#define MAX_AMPHIPODS 20

#define AT(i, j) ((i) * N_BUCKETS + (j))
#define CACHE_CAPACITY 300000
#define N_BUCKETS 100

struct amphipod {
  size_t row;
  size_t col;
  char val;
};

struct cache {
  struct amphipod *cache_amphipods;
  long *cache_values;
  size_t *cache_sizes;
};

static const size_t room_cols[4] = {2, 4, 6, 8};
static const size_t color_costs[4] = {1, 10, 100, 1000};

static int
in_room(struct amphipod);

static int
in_correct_room(struct amphipod);

static int
on_position(struct amphipod *, size_t, size_t, size_t);

static int
available_spots(struct amphipod amphipods[MAX_AMPHIPODS], struct amphipod,
                struct amphipod result[MAX_AMPHIPODS], size_t *);

static size_t
combine_hashes(size_t h1, size_t h2);

static size_t
numeric_hash(size_t x);

static size_t
hash(struct amphipod amphipods[MAX_AMPHIPODS]);

static int
amphipods_equal(struct amphipod a[MAX_AMPHIPODS],
                struct amphipod b[MAX_AMPHIPODS]);

static struct cache *
cache_new();

static long
cache_get(struct cache *, struct amphipod amphipods[MAX_AMPHIPODS]);

static void
cache_put(struct cache *, struct amphipod amphipods[MAX_AMPHIPODS], long);

static void
cache_free(struct cache *);

static long
solve(struct amphipod amphipods[MAX_AMPHIPODS], struct cache *);

static int
in_room(struct amphipod a)
{
  return a.row > 0;
}

static int
in_correct_room(struct amphipod a)
{
  return a.col == room_cols[a.val - 'A'];
}

static int
on_position(struct amphipod *amphipods, size_t n, size_t row, size_t col)
{
  size_t i;
  for (i = 0; i < n; ++i)
    if (amphipods[i].row == row && amphipods[i].col == col)
      return 1;
  return 0;
}

static int
available_spots(struct amphipod amphipods[MAX_AMPHIPODS], struct amphipod a, 
                struct amphipod result[MAX_AMPHIPODS], size_t *n)
{
  int below_done;
  size_t i, count, nvisible = 0, nmovable = 0;
  struct amphipod visible[MAX_AMPHIPODS], movable[MAX_AMPHIPODS];
  *n = 0;
  for (i = a.col + 1; i < 11; ++i) {
    if (on_position(amphipods, N_AMPHIPODS, 0, i))
      break;
    if (i != 2 && i != 4 && i != 6 && i != 8) {
      movable[nmovable].row = 0;
      movable[nmovable].col = i;
      movable[nmovable].val = a.val;
      nmovable++;
    }
    visible[nvisible].row = 0;
    visible[nvisible].col = i;
    visible[nvisible].val = a.val;
    nvisible++;
  }
  for (i = a.col; i > 0; --i) {
    if (on_position(amphipods, N_AMPHIPODS, 0, i - 1))
      break;
    if (i - 1 != 2 && i - 1 != 4 && i - 1 != 6 && i - 1 != 8) {
      movable[nmovable].row = 0;
      movable[nmovable].col = i - 1;
      movable[nmovable].val = a.val;
      nmovable++;
    }
    visible[nvisible].row = 0;
    visible[nvisible].col = i - 1;
    visible[nvisible].val = a.val;
    nvisible++;
  }
  if (in_room(a)) {
    if (in_correct_room(a)) {
      if (a.row == AMPHIPODS_PER_ROOM)
        return 0;
      below_done = 1;
      for (i = 0; i < N_AMPHIPODS && below_done; ++i) {
        if (amphipods[i].row > a.row && amphipods[i].col == a.col)
          below_done = below_done && amphipods[i].val == a.val;
      }
      if (below_done)
        return 0;
    }
    for (i = 0; i < N_AMPHIPODS; ++i)
      if (amphipods[i].row < a.row && amphipods[i].col == a.col)
        return 0;
    for (i = 0; i < nmovable; ++i)
      result[(*n)++] = movable[i];
    return 1;
  } else {
    if (!on_position(visible, nvisible, 0, room_cols[a.val - 'A']))
      return 0;
    count = 0;
    for (i = 0; i < N_AMPHIPODS; ++i) {
      if (amphipods[i].col == room_cols[a.val - 'A']) {
        if (amphipods[i].val != a.val)
          return 0;
        count++;
      }
    }
    result[*n].row = AMPHIPODS_PER_ROOM - count;
    result[*n].col = room_cols[a.val - 'A'];
    result[*n].val = a.val;
    (*n)++;
    return 1;
  }
}

static size_t
combine_hashes(size_t h1, size_t h2)
{
  return (((h1 << 5) + h1) ^ h2);
}

static size_t
numeric_hash(size_t x)
{
  x ^= x >> 17;
  x *= 0xed5ad4bbU;
  x ^= x >> 11;
  x *= 0xac4c1b51U;
  x ^= x >> 15;
  x *= 0x31848babU;
  x ^= x >> 14;
  return x;
}

static size_t
hash(struct amphipod amphipods[MAX_AMPHIPODS])
{
  size_t i, ret = 0;
  for (i = 0; i < N_AMPHIPODS; ++i) {
    ret = combine_hashes(ret, numeric_hash((size_t) amphipods[i].val));
    ret = combine_hashes(ret, numeric_hash(amphipods[i].row));
    ret = combine_hashes(ret, numeric_hash(amphipods[i].col));
  }
  return ret;
}

static int
amphipods_equal(struct amphipod a[MAX_AMPHIPODS],
                struct amphipod b[MAX_AMPHIPODS])
{
  size_t i;
  for (i = 0; i < N_AMPHIPODS; ++i)
    if (a[i].row != b[i].row || a[i].col != b[i].col || a[i].val != b[i].val)
      return 0;
  return 1;
}

static struct cache *
cache_new()
{
  struct cache *c = malloc(sizeof(struct cache));
  c->cache_amphipods = malloc(CACHE_CAPACITY * N_BUCKETS * MAX_AMPHIPODS
      * sizeof(struct amphipod));
  c->cache_values = malloc(CACHE_CAPACITY * N_BUCKETS * sizeof(long));
  c->cache_sizes = calloc(CACHE_CAPACITY, sizeof(size_t));
  return c;
}

static long
cache_get(struct cache *c, struct amphipod amphipods[MAX_AMPHIPODS])
{
  size_t i, h = hash(amphipods) % CACHE_CAPACITY;
  for (i = 0; i < c->cache_sizes[h]; ++i)
    if (amphipods_equal(amphipods, &(c->cache_amphipods[AT(h, i)])))
      return c->cache_values[AT(h, i)];
  return -1;
}

static void
cache_put(struct cache *c, struct amphipod amphipods[MAX_AMPHIPODS], long value)
{
  size_t h = hash(amphipods) % CACHE_CAPACITY;
  assert(c->cache_sizes[h] < N_BUCKETS);
  memcpy(&(c->cache_amphipods[AT(h, c->cache_sizes[h])]), amphipods,
      MAX_AMPHIPODS * sizeof(struct amphipod));
  c->cache_values[AT(h, c->cache_sizes[h])] = value;
  c->cache_sizes[h]++;
}

static void
cache_free(struct cache *c)
{
  free(c->cache_amphipods);
  free(c->cache_values);
  free(c->cache_sizes);
  free(c);
}

static long
solve(struct amphipod amphipods[MAX_AMPHIPODS], struct cache *c)
{
  int done = 1;
  size_t i, j, n_moves;
  long min_cost = LONG_MAX, cost = cache_get(c, amphipods);
  long dist, step_cost, rec;
  struct amphipod moves[MAX_AMPHIPODS], tmp;
  if (cost != -1)
    return cost;
  for (i = 0; i < N_AMPHIPODS; ++i)
    done = done && in_correct_room(amphipods[i]);
  if (done)
    return 0;
  for (i = 0; i < N_AMPHIPODS; ++i) {
    if (available_spots(amphipods, amphipods[i], moves, &n_moves)) {
      for (j = 0; j < n_moves; ++j) {
        tmp = amphipods[i];
        dist = ABS((long) moves[j].row - (long) tmp.row)
             + ABS((long) moves[j].col - (long) tmp.col);
        step_cost = dist * color_costs[tmp.val - 'A'];
        amphipods[i] = moves[j];
        rec = solve(amphipods, c);
        amphipods[i] = tmp;
        if (rec < LONG_MAX && step_cost + rec < min_cost)
          min_cost = step_cost + rec;
      }
    }
  }
  cache_put(c, amphipods, min_cost);
  return min_cost;
}

int
main(void)
{
  char buf[16] = {'\0'};
  long cost;
  size_t n = 0, row, col = 0;
  struct amphipod amphipods[MAX_AMPHIPODS];
  struct cache *c = cache_new();
  for (row = 0; fgets(buf, sizeof(buf), stdin); ++row) {
    for (col = 0; buf[col] != '\0' && buf[col] != '\n'; ++col) {
      if ('A' <= buf[col] && buf[col] <= 'D') {
        if (n == N_AMPHIPODS) {
          fprintf(stderr, "too many amphipods\n");
          exit(EXIT_FAILURE);
        }
        amphipods[n].row = row - 1;
        amphipods[n].col = col - 1;
        amphipods[n].val = buf[col];
        n++;
      }
    }
  }
  cost = solve(amphipods, c);
  cache_free(c);
  printf("%ld\n", cost);
  return EXIT_SUCCESS;
}
