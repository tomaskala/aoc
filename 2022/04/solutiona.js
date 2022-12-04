"use strict";

(function() {
  let fs = require("fs");

  function readLines(path) {
    return fs.readFileSync(path).toString().split("\n");
  }

  let lines = readLines("/dev/stdin");
  let fullyContained = 0;

  for (let line of lines) {
    if (line === "")
      continue;

    let pair = line.split(",");
    let fst = pair[0].split("-");
    let snd = pair[1].split("-");

    let left1 = parseInt(fst[0], 10);
    let right1 = parseInt(fst[1], 10);
    let left2 = parseInt(snd[0], 10);
    let right2 = parseInt(snd[1], 10);

    if ((left1 >= left2 && right1 <= right2)
      || (left2 >= left1 && right2 <= right1))
      fullyContained++;
  }

  console.log(fullyContained);
})();
