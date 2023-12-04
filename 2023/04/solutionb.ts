import { stdin } from "process";
import { createInterface } from "readline";

const rl = createInterface({
  input: stdin,
});

type Card = [number, Set<number>, Array<number>];

const whitespace = new RegExp("\\s+");
const cards: Array<Card> = [];

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

  cards.push([cards.length, winningNumbers, actualNumbers]);
});

rl.on("close", () => {
  const counts = [];

  for (let i = 0; i < cards.length; i++) {
    counts.push(1);
  }

  for (const [cardIdx, winningNumbers, actualNumbers] of cards) {
    const numWins = actualNumbers.filter((num) =>
      winningNumbers.has(num),
    ).length;

    for (let i = cardIdx + 1; i <= cardIdx + numWins; i++) {
      counts[i] += counts[cardIdx];
    }
  }

  console.log(counts.reduce((x, y) => x + y));
});
