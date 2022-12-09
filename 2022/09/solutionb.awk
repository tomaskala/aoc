BEGIN {
  for (i = 0; i < 10; i++) {
    ropex[i] = 0
    ropey[i] = 0
  }

  visited[0, 0] = true
}

function sign(x) {
  return x < 0 ? -1 : x > 0 ? 1 : 0
}

function max(x, y) {
  return x >= y ? x : y
}

function abs(x) {
  return max(x, -x)
}

function dist(i) {
  return max(abs(ropex[i - 1] - ropex[i]), abs(ropey[i - 1] - ropey[i]))
}

function move_tail() {
  for (k = 1; k < 10; k++) {
    if (dist(k) <= 1)
      return

    ropex[k] += sign(ropex[k - 1] - ropex[k])
    ropey[k] += sign(ropey[k - 1] - ropey[k])

    visited[ropex[9], ropey[9]] = true
  }
}

$1 == "R" {
  for (i = 0; i < $2; i++) {
    ropex[0]++
    move_tail()
  }
}

$1 == "L" {
  for (i = 0; i < $2; i++) {
    ropex[0]--
    move_tail()
  }
}

$1 == "U" {
  for (i = 0; i < $2; i++) {
    ropey[0]++
    move_tail()
  }
}

$1 == "D" {
  for (i = 0; i < $2; i++) {
    ropey[0]--
    move_tail()
  }
}

END {
  for (key in visited)
    total_visited++

  print total_visited
}
