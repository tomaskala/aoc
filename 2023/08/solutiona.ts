import { stdin } from "process";
import { createInterface } from "readline";

const rl = createInterface({
  input: stdin,
});

let lineIdx = 0;
const directions: Array<number> = [];
const graph: Map<string, [string, string]> = new Map();
const edge = new RegExp("(\\w{3}) = \\((\\w{3}), (\\w{3})\\)");

rl.on("line", (line) => {
  if (lineIdx == 0) {
    for (const char of line) {
      directions.push(char == "L" ? 0 : 1);
    }
  } else if (lineIdx > 1) {
    const [_, node, left, right] = line.match(edge)!;
    graph.set(node, [left, right]);
  }

  lineIdx++;
});

rl.on("close", () => {
  let steps = 0;
  let position = "AAA";

  while (position != "ZZZ") {
    const direction = directions[steps % directions.length];
    position = graph.get(position)![direction];
    steps++;
  }

  console.log(steps);
});
