import { stdin } from "process";
import { createInterface } from "readline";

const rl = createInterface({
  input: stdin,
});

const whitespace = new RegExp("\\s+");
let sum = 0;

rl.on("line", (line) => {
  const input = line.split(":")[1];
  const [winning, actual] = input.trim().split("|");

  const winningNumbers = new Set(
    winning
      .trim()
      .split(whitespace)
      .map((n) => parseInt(n)),
  );
  const actualNumbers = actual
    .trim()
    .split(whitespace)
    .map((n) => parseInt(n));

  const numWins = actualNumbers.filter((num) => winningNumbers.has(num)).length;
  sum += numWins == 0 ? 0 : Math.pow(2, numWins - 1);
});

rl.on("close", () => {
  console.log(sum);
});
