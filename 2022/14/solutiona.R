parse_input <- function() {
  segments <- lapply(
    strsplit(readLines(file("stdin")), split = " -> ", fixed = TRUE),
    function(l) {
      vapply(strsplit(l, split = ",", fixed = TRUE), as.integer, integer(2L)) + 1L
    })

  nc <- max(vapply(segments, function(seg) { max(seg[1L,]) }, integer(1L))) + 1L
  nr <- max(vapply(segments, function(seg) { max(seg[2L,]) }, integer(1L))) + 1L
  cave <- matrix(FALSE, nrow = nr, ncol = nc)

  for (s in segments) {
    for (i in seq_len(ncol(s) - 1L)) {
      cave[s[2L, i]:s[2L, i + 1L], s[1L, i]:s[1L, i + 1L]] <- TRUE
    }
  }

  return(cave)
}

next_grain <- function(cave) {
  grain <- c(1, 501)
  down <- c(1, 0)
  left <- c(1, -1)
  right <- c(1, 1)

  while (grain[1] < nrow(cave)) {
    if (!do.call(`[`, c(list(cave), grain + down))) {
      grain <- grain + down
    } else if (!do.call(`[`, c(list(cave), grain + left))) {
      grain <- grain + left
    } else if (!do.call(`[`, c(list(cave), grain + right))) {
      grain <- grain + right
    } else {
      break
    }
  }

  return(grain)
}

cave <- parse_input()
steps <- 0

while (TRUE) {
  grain <- next_grain(cave)
  cave <- do.call(`[<-`, c(list(cave), grain, TRUE))

  if (grain[1] == nrow(cave)) {
    break
  }

  steps <- steps + 1
}

print(steps)
