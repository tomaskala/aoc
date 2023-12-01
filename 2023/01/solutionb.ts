import { stdin } from "process";
import { createInterface } from "readline";

const digitsMap: Record<string, string> = {
  one: "1",
  two: "2",
  three: "3",
  four: "4",
  five: "5",
  six: "6",
  seven: "7",
  eight: "8",
  nine: "9",
};

function normalizeDigit(s: string): string {
  return digitsMap[s] || s;
}

const rl = createInterface({
  input: stdin,
});

const digitPattern = `(${Object.keys(digitsMap).join("|")}|\\d)`;
const firstPattern = new RegExp(digitPattern);
const lastPattern = new RegExp(`.*${digitPattern}`);
let sum = 0;

rl.on("line", (line) => {
  const first = line.match(firstPattern)![1];
  const last = line.match(lastPattern)![1];
  sum += parseInt(normalizeDigit(first) + normalizeDigit(last));
});

rl.on("close", () => {
  console.log(sum);
});
