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

function computeLcm(a: number, b: number): number {
  const x = a;
  const y = b;

  while (b != 0) {
    const t = b;
    b = a % b;
    a = t;
  }

  return (x * y) / a;
}

rl.on("close", () => {
  const startToSteps: Map<string, number> = new Map();

  for (const start of Array.from(graph.keys()).filter((node) =>
    node.endsWith("A"),
  )) {
    let steps = 0;
    let position = start;

    while (!position.endsWith("Z")) {
      const direction = directions[steps % directions.length];
      position = graph.get(position)![direction];
      steps++;
    }

    startToSteps.set(start, steps);
  }

  const steps: Array<number> = Array.from(startToSteps.values());
  const lcm = steps.reduce((x, y) => computeLcm(x, y));
  console.log(lcm);
});
