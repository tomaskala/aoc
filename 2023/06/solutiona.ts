import { stdin } from "process";
import { createInterface } from "readline";

const rl = createInterface({
  input: stdin,
});

function parseLine(line: string): Array<number> {
  return line
    .split(":")[1]
    .trim()
    .split(new RegExp("\\s+"))
    .map((n) => parseInt(n));
}

function distanceTravelled(timeMax: number, time: number): number {
  return time * timeMax - time * time;
}

function countWins(timeMax: number, distanceMax: number): number {
  const D = timeMax * timeMax - 4 * distanceMax;
  let t1 = Math.ceil((timeMax - Math.sqrt(D)) / 2);
  let t2 = Math.floor((timeMax + Math.sqrt(D)) / 2);

  if (distanceTravelled(timeMax, t1) == distanceMax) {
    t1++;
  }

  if (distanceTravelled(timeMax, t2) == distanceMax) {
    t2--;
  }

  return t2 - t1 + 1;
}

let times: Array<number> = [];
let distances: Array<number> = [];

rl.on("line", (line) => {
  if (line.startsWith("Time")) {
    times = parseLine(line);
  } else if (line.startsWith("Distance")) {
    distances = parseLine(line);
  }
});

rl.on("close", () => {
  let result = 1;

  for (let i = 0; i < times.length; i++) {
    const timeMax = times[i];
    const distanceMax = distances[i];
    const numWins = countWins(timeMax, distanceMax);
    result *= numWins;
  }

  console.log(result);
});
