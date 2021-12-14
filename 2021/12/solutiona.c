#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 64

struct graph {
  size_t n;
  size_t start;
  size_t end;
  int small[MAX_NODES];
  int visited[MAX_NODES];
  struct node *nodes[MAX_NODES];
};

struct node {
  char *id;
  size_t idx;
  struct node *next;
};

static struct graph *
graph_new();

static size_t
graph_add_node(struct graph *, char *);

static void
graph_add_edge(struct graph *, char *, char *);

static int
graph_count_paths(struct graph *);

static int
graph_dfs(struct graph *, size_t);

static void
graph_free(struct graph *);

static struct node *
node_new(char *);

static void
node_free(struct node *);

static struct graph *
graph_new()
{
  struct graph *g = malloc(sizeof(struct graph));
  g->n = 0;
  g->start = 0;
  g->end = 0;
  memset(g->visited, 0x00, MAX_NODES * sizeof(int));
  memset(g->nodes, 0x00, MAX_NODES * sizeof(struct node *));
  return g;
}

static size_t
graph_add_node(struct graph *g, char *id)
{
  size_t i;
  for (i = 0; i < g->n; ++i)
    if (g->nodes[i] != NULL && !strcmp(g->nodes[i]->id, id))
      return i;
  g->nodes[g->n] = node_new(id);
  g->nodes[g->n]->idx = g->n;
  g->small[g->n] = 1;
  for (i = 0; id[i]; ++i)
    if ('A' <= id[i] && id[i] <= 'Z') {
      g->small[g->n] = 0;
      break;
    }
  if (!strcmp(id, "start"))
    g->start = g->n;
  if (!strcmp(id, "end"))
    g->end = g->n;
  return g->n++;
}

static void
graph_add_edge(struct graph *g, char *id1, char *id2)
{
  struct node *n;
  size_t idx1 = graph_add_node(g, id1);
  size_t idx2 = graph_add_node(g, id2);
  for (n = g->nodes[idx1]; n->next != NULL; n = n->next);
  n->next = node_new(id2);
  n->next->idx = idx2;
  for (n = g->nodes[idx2]; n->next != NULL; n = n->next);
  n->next = node_new(id1);
  n->next->idx = idx1;
}

static int
graph_count_paths(struct graph *g)
{
  g->visited[g->start] = 1;
  return graph_dfs(g, g->start);
}

static int
graph_dfs(struct graph *g, size_t i)
{
  int total = 0;
  struct node *n;
  if (g->end == i)
    return 1;
  for (n = g->nodes[i]->next; n != NULL; n = n->next) {
    if (!g->visited[n->idx] || !g->small[n->idx]) {
      g->visited[n->idx] = 1;
      total += graph_dfs(g, n->idx);
      g->visited[n->idx] = 0;
    }
  }
  return total;
}

static void
graph_free(struct graph *g)
{
  size_t i;
  struct node *n, *tmp;
  for (i = 0; i < g->n; ++i) {
    n = g->nodes[i];
    while (n != NULL) {
      tmp = n;
      n = n->next;
      node_free(tmp);
    }
  }
  free(g);
}

static struct node *
node_new(char *id)
{
  size_t idlen;
  struct node *n;
  idlen = strlen(id);
  n = malloc(sizeof(struct node));
  n->id = calloc(idlen + 1, sizeof(char));
  memcpy(n->id, id, idlen);
  n->idx = 0;
  n->next = NULL;
  return n;
}

static void
node_free(struct node *n)
{
  free(n->id);
  free(n);
}

int
main(void)
{
  char buf[64] = {'\0'}, *tok1, *tok2;
  int paths;
  struct graph *g = graph_new();
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      if (!(tok1 = strtok(buf, "-"))) {
        fprintf(stderr, "cannot read the first token\n");
        exit(EXIT_FAILURE);
      }
      if (!(tok2 = strtok(NULL, "\n"))) {
        fprintf(stderr, "cannot read the second token\n");
        exit(EXIT_FAILURE);
      }
      graph_add_edge(g, tok1, tok2);
    }
  }
  paths = graph_count_paths(g);
  printf("%d\n", paths);
  graph_free(g);
  return EXIT_SUCCESS;
}
