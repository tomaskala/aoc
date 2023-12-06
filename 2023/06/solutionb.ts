import { stdin } from "process";
import { createInterface } from "readline";

const rl = createInterface({
  input: stdin,
});

function parseLine(line: string): number {
  return parseInt(line.split(":")[1].trim().replace(new RegExp("\\s+", "g"), ""));
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

let timeMax: number = 0;
let distanceMax: number = 0;

rl.on("line", (line) => {
  if (line.startsWith("Time")) {
    timeMax = parseLine(line);
  } else if (line.startsWith("Distance")) {
    distanceMax = parseLine(line);
  }
});

rl.on("close", () => {
  const result = countWins(timeMax, distanceMax);
  console.log(result);
});
