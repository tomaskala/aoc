import { stdin } from "process";
import { createInterface } from "readline";

const rl = createInterface({
  input: stdin,
});

let sum = 0;

function diff(sequence: Array<number>): void {
  let prev = sequence[0];
  let curr;

  for (let i = 1; i < sequence.length; i++) {
    curr = sequence[i];
    sequence[i - 1] = sequence[i] - prev;
    prev = curr;
  }

  sequence.pop();
}

rl.on("line", (line) => {
  const sequence = line
    .trim()
    .split(" ")
    .map((_, i, arr) => parseInt(arr[arr.length - 1 - i]));

  while (!sequence.every((n) => n == 0)) {
    sum += sequence[sequence.length - 1];
    diff(sequence);
  }
});

rl.on("close", () => {
  console.log(sum);
});
