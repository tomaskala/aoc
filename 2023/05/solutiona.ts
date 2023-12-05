import { stdin } from "process";
import { createInterface } from "readline";

const rl = createInterface({
  input: stdin,
});

type Mapping = {
  destStart: number;
  sourceStart: number;
  rangeLength: number;
};

class Almanac {
  maps: Array<Array<Mapping>> = [];

  addMap(map: Array<Mapping>): void {
    this.maps.push(map);
  }

  pass(seed: number): number {
    let result = seed;

    for (const map of this.maps) {
      result = this.passThroughMap(result, map);
    }

    return result;
  }

  private passThroughMap(seed: number, map: Array<Mapping>): number {
    for (const mapping of map) {
      if (
        seed >= mapping.sourceStart &&
        seed < mapping.sourceStart + mapping.rangeLength
      ) {
        return seed - mapping.sourceStart + mapping.destStart;
      }
    }

    return seed;
  }
}

let seeds: Array<number> = [];
let currentMap: Array<Mapping> = [];
const almanac = new Almanac();

rl.on("line", (line) => {
  if (line.startsWith("seeds")) {
    seeds = line
      .split(":")[1]
      .trim()
      .split(" ")
      .map((n) => parseInt(n));
  } else if (line.trim() !== "") {
    const head = line.substring(0, line.indexOf(" "));

    if (isNaN(parseInt(head))) {
      if (currentMap.length > 0) {
        almanac.addMap(currentMap);
        currentMap = [];
      }
    } else {
      const [destStart, sourceStart, rangeLength] = line.split(" ");
      currentMap.push({
        destStart: parseInt(destStart),
        sourceStart: parseInt(sourceStart),
        rangeLength: parseInt(rangeLength),
      });
    }
  }
});

rl.on("close", () => {
  almanac.addMap(currentMap);
  let min = Infinity;

  for (const seed of seeds) {
    const value = almanac.pass(seed);
    min = Math.min(min, value);
  }

  console.log(min);
});
