import { stdin } from "process";
import { createInterface } from "readline";

const rl = createInterface({
  input: stdin,
});

type Coords = [number, number];

function findStart(lines: Array<string>): Coords | undefined {
  for (let i = 0; i < lines.length; i++) {
    for (let j = 0; j < lines[i].length; j++) {
      if (lines[i][j] == "S") {
        return [i, j];
      }
    }
  }

  return undefined;
}

function bfs(lines: Array<string>, start: Coords): number {
  const queue: Array<[number, Coords]> = [];
  for (const [dy, dx] of [
    [-1, 0],
    [1, 0],
    [0, -1],
    [0, 1],
  ]) {
    const neighbor = lines[start[0] + dy][start[1] + dx];

    if (neighbor in delta) {
      for (const [dy2, dx2] of delta[neighbor]) {
        // Skip neighbors of the start position that are valid map positions
        // but do not lead from/to the start tile.
        if (dx == -dx2 && dy == -dy2) {
          queue.push([1, [start[0] + dy, start[1] + dx]]);
        }
      }
    }
  }

  // Keys have to be compared by identity.
  const dists: Map<string, number> = new Map();
  dists.set(`${start[0]},${start[1]}`, 0);

  while (queue.length > 0) {
    const [d, [y, x]] = queue.shift() as [number, Coords];
    const key = `${y},${x}`;
    if (dists.has(key)) {
      continue;
    }

    dists.set(key, d);
    for (const [dy, dx] of delta[lines[y][x]]) {
      queue.push([d + 1, [y + dy, x + dx]]);
    }
  }

  return Math.max(...dists.values());
}

const lines: Array<string> = [];
const delta: Record<string, Array<Coords>> = {
  "|": [
    [-1, 0],
    [1, 0],
  ],
  "-": [
    [0, -1],
    [0, 1],
  ],
  L: [
    [-1, 0],
    [0, 1],
  ],
  J: [
    [-1, 0],
    [0, -1],
  ],
  "7": [
    [1, 0],
    [0, -1],
  ],
  F: [
    [1, 0],
    [0, 1],
  ],
};

rl.on("line", (line) => {
  lines.push(line.trim());
});

rl.on("close", () => {
  const startCoords = findStart(lines);
  if (startCoords === undefined) {
    console.log("Start coordinates not found");
    process.exit(1);
  }

  console.log(bfs(lines, startCoords));
});
