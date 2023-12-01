import { stdin } from "process";
import { createInterface } from "readline";

const rl = createInterface({
  input: stdin,
});

const digitPattern = "(\\d)";
const firstPattern = new RegExp(digitPattern);
const lastPattern = new RegExp(`.*${digitPattern}`);
let sum = 0;

rl.on("line", (line) => {
  const first = line.match(firstPattern)![1];
  const last = line.match(lastPattern)![1];
  sum += parseInt(first + last);
});

rl.on("close", () => {
  console.log(sum);
});
