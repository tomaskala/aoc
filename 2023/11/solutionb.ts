import { stdin } from "process";
import { createInterface } from "readline";

const rl = createInterface({
  input: stdin,
});

function allGalaxies(grid: Array<Array<string>>): Array<[number, number]> {
  const galaxies: Array<[number, number]> = [];

  for (let i = 0; i < grid.length; i++) {
    for (let j = 0; j < grid[i].length; j++) {
      if (grid[i][j] == "#") {
        galaxies.push([i, j]);
      }
    }
  }

  return galaxies;
}

function* unorderedPairs(
  coords: Array<[number, number]>,
): Generator<[[number, number], [number, number]]> {
  for (const [i, coords1] of coords.entries()) {
    for (const [j, coords2] of coords.entries()) {
      if (j > i) {
        yield [coords1, coords2];
      }
    }
  }
}

const grid: Array<Array<string>> = [];
const expansion = 1_000_000;

rl.on("line", (line) => {
  grid.push(line.trim().split(""));
});

rl.on("close", () => {
  const gridT = grid[0].map((_, j) => grid.map((row) => row[j]));

  const galaxies = allGalaxies(grid);
  const emptyRows = Array.from(new Array(grid.length), (_, i) => i).filter(
    (i) => grid[i].every((c) => c == "."),
  );
  const emptyCols = Array.from(new Array(gridT.length), (_, i) => i).filter(
    (j) => gridT[j].every((c) => c == "."),
  );

  let sum = 0;

  for (const [[i1, j1], [i2, j2]] of unorderedPairs(galaxies)) {
    sum += Math.abs(i2 - i1) + Math.abs(j2 - j1);

    for (const i of emptyRows) {
      if (Math.min(i1, i2) < i && i < Math.max(i1, i2)) {
        sum += expansion - 1;
      }
    }

    for (const j of emptyCols) {
      if (Math.min(j1, j2) < j && j < Math.max(j1, j2)) {
        sum += expansion - 1;
      }
    }
  }

  console.log(sum);
});
