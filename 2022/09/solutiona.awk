function sign(x) {
  return x < 0 ? -1 : x > 0 ? 1 : 0
}

function max(x, y) {
  return x >= y ? x : y
}

function abs(x) {
  return max(x, -x)
}

function dist() {
  return max(abs(ropex[0] - ropex[1]), abs(ropey[0] - ropey[1]))
}

function move_tail() {
  if (dist() <= 1)
    return

  ropex[1] += sign(ropex[0] - ropex[1])
  ropey[1] += sign(ropey[0] - ropey[1])

  visited[ropex[1], ropey[1]] = true
}

BEGIN {
  ropex[0] = 0
  ropey[0] = 0
  ropex[1] = 0
  ropey[1] = 0
  visited[0, 0] = true
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
