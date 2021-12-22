#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SCANNERS 32
#define MAX_BEACONS 30
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ROW(i) ((i) * (3))
#define AT(i, j) ((i) * (3) + (j))

struct scanner {
  size_t nbeacons;
  size_t capacity;
  int *beacons;
};

static struct scanner *
scanner_new(size_t);

static void
scanner_rotate(int, struct scanner *, struct scanner *);

static void
scanner_translate(struct scanner *, int vec[3], struct scanner *);

static int
scanner_contains(struct scanner *, int vec[3]);

static int
scanners_intersection(struct scanner *, struct scanner *);

static void
scanner_free(struct scanner *);

static int
scanners_match(struct scanner *, struct scanner *, struct scanner *,
               struct scanner *);

static void
scanners_merge(struct scanner *, struct scanner *);

static void
rotate(int, int point[3], int out[3]);

static long
parse_num(char *, char *);

static struct scanner *
scanner_new(size_t capacity)
{
  struct scanner *s = malloc(sizeof(struct scanner));
  s->nbeacons = 0;
  s->capacity = capacity;
  s->beacons = malloc(capacity * 3 * sizeof(int));
  return s;
}

static void
scanner_rotate(int r, struct scanner *s, struct scanner *out)
{
  size_t i;
  assert(s->nbeacons <= out->capacity);
  for (i = 0; i < s->nbeacons; ++i)
    rotate(r, &s->beacons[ROW(i)], &out->beacons[ROW(i)]);
}

static void
scanner_translate(struct scanner *s, int vec[3], struct scanner *out)
{
  size_t i;
  assert(s->nbeacons <= out->capacity);
  for (i = 0; i < s->nbeacons; ++i) {
    out->beacons[AT(i, 0)] = s->beacons[AT(i, 0)] + vec[0];
    out->beacons[AT(i, 1)] = s->beacons[AT(i, 1)] + vec[1];
    out->beacons[AT(i, 2)] = s->beacons[AT(i, 2)] + vec[2];
  }
}

static int
scanner_contains(struct scanner *s, int vec[3])
{
  size_t i;
  for (i = 0; i < s->nbeacons; ++i)
    if (s->beacons[AT(i, 0)] == vec[0]
        && s->beacons[AT(i, 1)] == vec[1]
        && s->beacons[AT(i, 2)] == vec[2])
      return 1;
  return 0;
}

static int
scanners_intersection(struct scanner *s1, struct scanner *s2)
{
  int intersection = 0;
  size_t i, j;
  for (i = 0; i < s1->nbeacons; ++i)
    for (j = 0; j < s2->nbeacons; ++j)
      if (s1->beacons[AT(i, 0)] == s2->beacons[AT(j, 0)]
          && s1->beacons[AT(i, 1)] == s2->beacons[AT(j, 1)]
          && s1->beacons[AT(i, 2)] == s2->beacons[AT(j, 2)])
        intersection++;
  return intersection;
}

static void
scanner_free(struct scanner *s)
{
  free(s->beacons);
  free(s);
}

static int
scanners_match(struct scanner *s1, struct scanner *s2, struct scanner *t,
               struct scanner *work)
{
  int r, delta[3];
  size_t i, j;
  assert(s2->nbeacons <= t->capacity);
  assert(s2->nbeacons <= work->capacity);
  for (r = 0; r < 24; ++r) {
    scanner_rotate(r, s2, work);
    work->nbeacons = s2->nbeacons;
    for (i = 0; i < s1->nbeacons; ++i) {
      for (j = 0; j < s2->nbeacons; ++j) {
        delta[0] = s1->beacons[AT(i, 0)] - work->beacons[AT(j, 0)];
        delta[1] = s1->beacons[AT(i, 1)] - work->beacons[AT(j, 1)];
        delta[2] = s1->beacons[AT(i, 2)] - work->beacons[AT(j, 2)];
        scanner_translate(work, delta, t);
        t->nbeacons = work->nbeacons;
        if (scanners_intersection(s1, t) >= 12)
          return 1;
      }
    }
  }
  return 0;
}

static void
scanners_merge(struct scanner *to, struct scanner *from)
{
  size_t i;
  assert(to->capacity - to->nbeacons >= from->nbeacons);
  for (i = 0; i < from->nbeacons; ++i)
    if (!scanner_contains(to, &from->beacons[ROW(i)])) {
      to->beacons[AT(to->nbeacons, 0)] = from->beacons[AT(i, 0)];
      to->beacons[AT(to->nbeacons, 1)] = from->beacons[AT(i, 1)];
      to->beacons[AT(to->nbeacons, 2)] = from->beacons[AT(i, 2)];
      to->nbeacons++;
    }
}

static void
rotate(int r, int point[3], int out[3])
{
  int tmp;
  assert(0 <= r && r < 24);
  switch (r / 4) {
  case 0:
    out[0] = point[0];
    out[1] = point[1];
    out[2] = point[2];
    break;
  case 1:
    out[0] = -point[0];
    out[1] = point[1];
    out[2] = -point[2];
    break;
  case 2:
    out[0] = point[2];
    out[1] = point[1];
    out[2] = -point[0];
    break;
  case 3:
    out[0] = -point[2];
    out[1] = point[1];
    out[2] = point[0];
    break;
  case 4:
    out[0] = point[0];
    out[1] = point[2];
    out[2] = -point[1];
    break;
  case 5:
    out[0] = point[0];
    out[1] = -point[2];
    out[2] = point[1];
    break;
  }
  switch (r % 4) {
  case 0:
    break;
  case 1:
    tmp = out[0];
    out[0] = -out[1];
    out[1] = tmp;
    break;
  case 2:
    out[0] = -out[0];
    out[1] = -out[1];
    break;
  case 3:
    tmp = out[0];
    out[0] = out[1];
    out[1] = -tmp;
    break;
  }
}

static long
parse_num(char *str, char *delim)
{
  char *tok;
  long res;
  if (!(tok = strtok(str, delim))) {
    fprintf(stderr, "cannot tokenize input\n");
    exit(EXIT_FAILURE);
  }
  errno = 0;
  res = strtol(tok, NULL, 10);
  if (errno) {
    fprintf(stderr, "cannot parse a number\n");
    exit(EXIT_FAILURE);
  }
  return res;
}

int
main(void)
{
  char buf[32] = {'\0'};
  size_t i = 0, nscanners = 0, nprocessed = 1, max_beacons = 0;
  int processed[MAX_SCANNERS] = {0};
  struct scanner *scanners[MAX_SCANNERS];
  struct scanner *t, *work;
  scanners[0] = scanner_new(MAX_BEACONS * MAX_SCANNERS);
  for (i = 1; i < MAX_SCANNERS; ++i)
    scanners[i] = scanner_new(MAX_BEACONS);
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      if (buf[0] == '\n') {
        nscanners++;
        continue;
      } else if (strstr(buf, "---")) {
        i = 0;
      } else {
        scanners[nscanners]->beacons[AT(i, 0)] = parse_num(buf, ",");
        scanners[nscanners]->beacons[AT(i, 1)] = parse_num(NULL, ",");
        scanners[nscanners]->beacons[AT(i, 2)] = parse_num(NULL, "\n");
        scanners[nscanners]->nbeacons++;
        max_beacons = MAX(max_beacons, scanners[nscanners]->nbeacons);
        i++;
      }
    }
  }
  nscanners++;
  t = scanner_new(max_beacons);
  work = scanner_new(max_beacons);
  processed[0] = 1;
  while (nprocessed < nscanners) {
    for (i = 1; i < nscanners; ++i) {
      if (processed[i])
        continue;
      if (scanners_match(scanners[0], scanners[i], t, work)) {
        scanners_merge(scanners[0], t);
        processed[i] = 1;
        nprocessed++;
      }
    }
  }
  printf("%ld\n", scanners[0]->nbeacons);
  for (i = 0; i < MAX_SCANNERS; ++i)
    scanner_free(scanners[i]);
  scanner_free(t);
  scanner_free(work);
  return EXIT_SUCCESS;
}
