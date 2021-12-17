#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 1024
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

enum length_type_id {
  BITS = 0,
  PACKETS = 1
};

enum packet_type {
  SUM = 0,
  PRODUCT = 1,
  MINIMUM = 2,
  MAXIMUM = 3,
  LITERAL = 4,
  GREATER = 5,
  LESS = 6,
  EQUAL = 7
};

struct packet {
  uint32_t version;
  enum packet_type type;
  union {
    uint64_t literal;
    struct node *children;
  } payload;
};

struct node {
  struct packet *p;
  struct node *next;
};

struct parser_state {
  size_t offset;
  size_t len;
  uint8_t *bs;
};

static struct parser_state *
state_new(uint8_t bs[MAX_LEN], size_t len);

static void
state_free(struct parser_state *state);

static struct packet *
packet_new(uint32_t version, enum packet_type type);

static struct packet *
packet_read(struct parser_state *state);

static uint64_t
packet_eval(struct packet *p);

static void
packet_free(struct packet *p);

static struct node *
node_new(struct packet *p);

static void
node_free(struct node *n);

static uint64_t
read_literal(struct parser_state *state);

static struct node *
read_operator(struct parser_state *state);

static uint32_t
read_bits(struct parser_state *state, size_t n);

static struct parser_state *
state_new(uint8_t bs[MAX_LEN], size_t len)
{
  struct parser_state *state = malloc(sizeof(struct parser_state));
  state->offset = 0;
  state->len = len;
  state->bs = bs;
  return state;
}

static void
state_free(struct parser_state *state)
{
  free(state);
}

static struct packet *
packet_new(uint32_t version, enum packet_type type)
{
  struct packet *p;
  assert(SUM <= type && type <= EQUAL);
  p = malloc(sizeof(struct packet));
  p->version = version;
  p->type = type;
  return p;
}

static struct packet *
packet_read(struct parser_state *state)
{
  uint32_t version, type_id;
  struct packet *p;
  version = read_bits(state, 3);
  type_id = read_bits(state, 3);
  switch (type_id) {
  case LITERAL:
    p = packet_new(version, LITERAL);
    p->payload.literal = read_literal(state);
    break;
  default:
    p = packet_new(version, type_id);
    p->payload.children = read_operator(state);
    break;
  }
  return p;
}

static uint64_t
packet_eval(struct packet *p)
{
  uint64_t tmp, result;
  struct node *n;
  switch (p->type) {
  case SUM:
    result = 0;
    for (n = p->payload.children; n != NULL; n = n->next)
      result += packet_eval(n->p);
    break;
  case PRODUCT:
    result = 1;
    for (n = p->payload.children; n != NULL; n = n->next)
      result *= packet_eval(n->p);
    break;
  case MINIMUM:
    result = UINT64_MAX;
    for (n = p->payload.children; n != NULL; n = n->next) {
      tmp = packet_eval(n->p);
      result = MIN(result, tmp);
    }
    break;
  case MAXIMUM:
    result = 0;
    for (n = p->payload.children; n != NULL; n = n->next) {
      tmp = packet_eval(n->p);
      result = MAX(result, tmp);
    }
    break;
  case LITERAL:
    return p->payload.literal;
  case GREATER:
    n = p->payload.children;
    assert(n != NULL);
    result = packet_eval(n->p);
    n = n->next;
    assert(n != NULL);
    result = result < packet_eval(n->p);
    assert(n->next == NULL);
    break;
  case LESS:
    n = p->payload.children;
    assert(n != NULL);
    result = packet_eval(n->p);
    n = n->next;
    assert(n != NULL);
    result = result > packet_eval(n->p);
    assert(n->next == NULL);
    break;
  case EQUAL:
    n = p->payload.children;
    assert(n != NULL);
    result = packet_eval(n->p);
    n = n->next;
    assert(n != NULL);
    result = result == packet_eval(n->p);
    assert(n->next == NULL);
    break;
  default:
    assert(0);
  }
  return result;
}

static void
packet_free(struct packet *p)
{
  switch (p->type) {
  case LITERAL:
    break;
  default:
    node_free(p->payload.children);
    break;
  }
  free(p);
}

static struct node *
node_new(struct packet *p)
{
  struct node *n = malloc(sizeof(struct node));
  n->p = p;
  n->next = NULL;
  return n;
}

static void
node_free(struct node *n)
{
  struct node *tmp;
  while (n != NULL) {
    tmp = n;
    n = n->next;
    packet_free(tmp->p);
    free(tmp);
  }
}

static uint64_t
read_literal(struct parser_state *state)
{
  uint32_t i;
  uint64_t payload = 0;
  while ((i = read_bits(state, 5)) & 16)
    payload = (payload << 4) | (i & 15);
  payload = (payload << 4) | (i & 15);
  return payload;
}

static struct node *
read_operator(struct parser_state *state)
{
  uint32_t i, bound, id = read_bits(state, 1);
  struct packet *p;
  struct node *head = NULL, *n;
  switch (id) {
  case BITS:
    bound = read_bits(state, 15);
    bound += state->offset;
    while (state->offset < bound) {
      p = packet_read(state);
      n = node_new(p);
      n->next = head;
      head = n;
    }
    break;
  case PACKETS:
    bound = read_bits(state, 11);
    for (i = 0; i < bound; ++i) {
      p = packet_read(state);
      n = node_new(p);
      n->next = head;
      head = n;
    }
    break;
  }
  return head;
}

static uint32_t
read_bits(struct parser_state *state, size_t n)
{
  size_t byte_offset = state->offset / 8;
  uint32_t bits, mask = (1 << n) - 1;
  assert(byte_offset + 3 < state->len);
  bits =  state->bs[byte_offset + 0] << 24;
  bits |= state->bs[byte_offset + 1] << 16;
  bits |= state->bs[byte_offset + 2] << 8;
  bits |= state->bs[byte_offset + 3] << 0;
  bits >>= 32 - (n + state->offset % 8);
  bits &= mask;
  state->offset += n;
  return bits;
}

int
main(void)
{
  char c, buf[3] = {'\0'};
  uint8_t bs[MAX_LEN];
  uint64_t result;
  size_t len = 0;
  struct parser_state *state;
  struct packet *p;
  while (1) {
    if (len == MAX_LEN - 4) {
      fprintf(stderr, "the input is too long\n");
      exit(EXIT_FAILURE);
    }
    if ((c = fgetc(stdin)) == EOF)
      break;
    buf[0] = c;
    if ((c = fgetc(stdin)) == EOF) {
      buf[1] = buf[0];
      buf[0] = 0;
      break;
    }
    buf[1] = c;
    errno = 0;
    bs[len++] = (uint8_t) strtol(buf, NULL, 16);
    if (errno) {
      fprintf(stderr, "cannot parse a number from %s\n", buf);
      exit(EXIT_FAILURE);
    }
  }
  len += 4;
  state = state_new(bs, len);
  p = packet_read(state);
  result = packet_eval(p);
  packet_free(p);
  state_free(state);
  printf("%lu\n", result);
  return EXIT_SUCCESS;
}
