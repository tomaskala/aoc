import { stdin } from "process";
import { createInterface } from "readline";

const rl = createInterface({
  input: stdin,
});

const RED = 12;
const GREEN = 13;
const BLUE = 14;

type Color = "red" | "green" | "blue";
type Bag = Record<Color, number>;

class Game {
  private bags: ReadonlyArray<Bag>;

  constructor(
    public readonly gameId: number,
    bags: ReadonlyArray<Bag>,
  ) {
    this.bags = bags;
  }

  isValid(): boolean {
    return this.bags.every(
      (bag) => bag.red <= RED && bag.green <= GREEN && bag.blue <= BLUE,
    );
  }

  static parse(gameStr: string): Game {
    const parts = gameStr.split(":");
    const gameId = parseInt(parts[0].split(" ")[1]);
    const bags = [];

    for (const bagStr of parts[1].split(";")) {
      const bag = { red: 0, green: 0, blue: 0 };

      for (const colorStr of bagStr.split(",")) {
        const [num, color] = colorStr.trim().split(" ");
        bag[color as Color] = parseInt(num);
      }

      bags.push(bag);
    }

    return new Game(gameId, bags);
  }
}

let sum = 0;

rl.on("line", (line) => {
  const game = Game.parse(line);

  if (game.isValid()) {
    sum += game.gameId;
  }
});

rl.on("close", () => {
  console.log(sum);
});
