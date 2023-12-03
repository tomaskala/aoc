import { stdin } from "process";
import { createInterface } from "readline";

const rl = createInterface({
  input: stdin,
});

interface NumberRange {
  line: number;
  start: number;
  end: number;
}

type Board = Array<string>;
type Numbers = Array<NumberRange>;

function isDigit(s: string): boolean {
  return s >= "0" && s <= "9";
}

function isSymbol(s: string): boolean {
  return s != "." && !isDigit(s);
}

function isPartNumber(board: Board, nr: NumberRange): boolean {
  const topBound = Math.max(nr.line - 1, 0);
  const bottomBound = Math.min(nr.line + 2, board.length);
  const leftBound = Math.max(nr.start - 1, 0);
  const rightBound = Math.min(nr.end + 1, board[nr.line].length);

  for (let i = topBound; i < bottomBound; i++) {
    for (let j = leftBound; j < rightBound; j++) {
      if (isSymbol(board[i][j])) {
        return true;
      }
    }
  }

  return false;
}

const board: Board = [];
const numberRanges: Numbers = [];
let lineNum = 0;
let sum = 0;

rl.on("line", (line) => {
  for (let i = 0; i < line.length; i++) {
    if (isDigit(line[i])) {
      const start = i;

      while (isDigit(line[i])) {
        i++;
      }

      numberRanges.push({ line: lineNum, start: start, end: i });
    }
  }

  board.push(line);
  lineNum++;
});

rl.on("close", () => {
  for (const nr of numberRanges) {
    if (isPartNumber(board, nr)) {
      sum += parseInt(board[nr.line].substring(nr.start, nr.end));
    }
  }

  console.log(sum);
});
