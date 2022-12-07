<?php

class Node {
  public function __construct($parent) {
    $this->parent = $parent;
    $this->children = array();
    $this->size = 0;
  }

  public function count_directory_sizes() {
    foreach ($this->children as $k => $v) {
      $v->count_directory_sizes();
      $this->size += $v->size;
    }
  }

  public function find_small_directories() {
    $size = $this->is_directory() && $this->size <= 100000 ? $this->size : 0;

    foreach ($this->children as $k => $v) {
      $size += $v->find_small_directories();
    }

    return $size;
  }

  private function is_directory() {
    return count($this->children) > 0;
  }
}

$root = new Node(NULL);
$node = $root;

while ($line = fgets(STDIN)) {
  $split = array_map("trim", explode(" ", $line));

  if ($split[0] == "$" && $split[1] == "cd") {
    if ($split[2] == "/") {
      $node = $root;
    } else if ($split[2] == "..") {
      $node = $node->parent == NULL ? $root : $node->parent;
    } else {
      if (!array_key_exists($split[2], $node->children)) {
        $node->children[$split[2]] = new Node($node);
      }

      $node = $node->children[$split[2]];
    }
  } else if ($split[0] != "$") {
    if (!array_key_exists($split[1], $node->children)) {
      $node->children[$split[1]] = new Node($node);
    }

    if ($split[0] != "dir") {
      $node->children[$split[1]]->size = $split[0];
    }
  }
}

$root->count_directory_sizes();
echo "{$root->find_small_directories()}\n";
