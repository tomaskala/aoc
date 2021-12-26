#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 64
#define IS_NUMBER(n) (((n)->left == NULL) && ((n)->right == NULL))

struct node {
  long value;
  struct node *parent;
  struct node *left;
  struct node *right;
};

static struct node *
node_new(long, struct node *);

static void
node_free(struct node *);

static struct node *
parse_tree(char buf[MAX_LEN]);

static struct node *
add(struct node *, struct node *);

static int
explode(struct node *, int);

static int
split(struct node *);

static struct node *
node_new(long value, struct node *parent)
{
  struct node *n = malloc(sizeof(struct node));
  n->value = value;
  n->parent = parent;
  n->left = NULL;
  n->right = NULL;
  return n;
}

static void
node_free(struct node *n)
{
  if (n == NULL)
    return;
  if (!IS_NUMBER(n)) {
    node_free(n->left);
    node_free(n->right);
  }
  free(n);
}

static struct node *
parse_tree(char buf[MAX_LEN])
{
  size_t i, j = 0;
  struct node *n1 = NULL, *n2 = NULL;
  struct node *stack[MAX_LEN] = {NULL};
  for (i = 0; buf[i] != '\0' && buf[i] != '\n'; ++i) {
    assert(j < MAX_LEN - 1);
    switch (buf[i]) {
    case '[':
      stack[++j] = node_new(0, NULL);
      break;
    case ']':
      assert(j > 1);
      n1 = stack[j--];
      n2 = stack[j--];
      n1->parent = n2;
      n2->right = n1;
      stack[++j] = n2;
      break;
    case ',':
      assert(j > 1);
      n1 = stack[j--];
      n2 = stack[j--];
      n1->parent = n2;
      n2->left = n1;
      stack[++j] = n2;
      break;
    default:
      if ('0' <= buf[i] && buf[i] <= '9')
        stack[++j] = node_new(buf[i] - '0', NULL);
      else {
        fprintf(stderr, "cannot parse a number from %c\n", buf[i]);
        goto cleanup;
      }
      break;
    }
  }
  if (j != 1) {
    fprintf(stderr, "invalid input\n");
    goto cleanup;
  }
  return stack[j];
cleanup:
  for (i = 0; i <= j; ++i)
    if (stack[i] != NULL)
      node_free(stack[i]);
  return NULL;
}

static struct node *
add(struct node *a, struct node *b)
{
  struct node *root;
  if (a == NULL)
    return b;
  if (b == NULL)
    return a;
  root = node_new(0, NULL);
  root->left = a;
  a->parent = root;
  root->right = b;
  b->parent = root;
  while (explode(root, 0) || split(root));
  return root;
}

static int
explode(struct node *root, int depth)
{
  struct node *n;
  if (IS_NUMBER(root))
    return 0;
  else if (depth == 4) {
    assert(IS_NUMBER(root->left) && IS_NUMBER(root->right));
    n = root;
    while (n->parent != NULL && n->parent->left == n)
      n = n->parent;
    if (n->parent != NULL) {
      n = n->parent->left;
      while (n->right != NULL)
        n = n->right;
      n->value += root->left->value;
    }
    n = root;
    while (n->parent != NULL && n->parent->right == n)
      n = n->parent;
    if (n->parent != NULL) {
      n = n->parent->right;
      while (n->left != NULL)
        n = n->left;
      n->value += root->right->value;
    }
    node_free(root->left);
    root->left = NULL;
    node_free(root->right);
    root->right = NULL;
    return 1;
  } else
    return explode(root->left, depth + 1) || explode(root->right, depth + 1);
}

static int
split(struct node *root)
{
  if (IS_NUMBER(root)) {
    if (root->value >= 10) {
      root->left = node_new(root->value / 2, root);
      root->right = node_new(root->value / 2 + root->value % 2, root);
      root->value = 0;
      return 1;
    } else
      return 0;
  } else
    return split(root->left) || split(root->right);
}

static long
magnitude(struct node *n)
{
  if (IS_NUMBER(n))
    return n->value;
  return 3 * magnitude(n->left) + 2 * magnitude(n->right);
}

int
main(void)
{
  char buf[MAX_LEN] = {'\0'};
  long result;
  struct node *a = NULL, *b = NULL;
  while (!feof(stdin)) {
    if (fgets(buf, sizeof(buf), stdin)) {
      b = parse_tree(buf);
      if (b == NULL) {
        fprintf(stderr, "cannot parse a tree from %s", buf);
        exit(EXIT_FAILURE);
      }
      a = add(a, b);
    }
  }
  result = magnitude(a);
  node_free(a);
  printf("%ld\n", result);
  return EXIT_SUCCESS;
}
