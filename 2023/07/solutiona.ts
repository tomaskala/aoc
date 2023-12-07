import { stdin } from "process";
import { createInterface } from "readline";

const rl = createInterface({
  input: stdin,
});

type Hand = {
  cards: Array<string>;
  bid: number;
};

// Sorted in ascending order for comparison.
const FIVE_OF_A_KIND = "5";
const FOUR_OF_A_KIND = "14";
const FULL_HOUSE = "23";
const THREE_OF_A_KIND = "113";
const TWO_PAIR = "122";
const ONE_PAIR = "1112";
const HIGH_CARD = "11111";

const CARD_RANKS: Record<string, number> = {
  A: 12,
  K: 11,
  Q: 10,
  J: 9,
  T: 8,
  "9": 7,
  "8": 6,
  "7": 5,
  "6": 4,
  "5": 3,
  "4": 2,
  "3": 1,
  "2": 0,
};

function calculateRank(hand: Hand): number {
  const handType = Array.from(
    hand.cards
      .reduce(
        (acc, card) => acc.set(card, (acc.get(card) || 0) + 1),
        new Map<string, number>(),
      )
      .values(),
  );
  handType.sort();

  // Arrays couldn't be compared by value.
  switch (handType.join("")) {
    case FIVE_OF_A_KIND:
      return 6;
    case FOUR_OF_A_KIND:
      return 5;
    case FULL_HOUSE:
      return 4;
    case THREE_OF_A_KIND:
      return 3;
    case TWO_PAIR:
      return 2;
    case ONE_PAIR:
      return 1;
    case HIGH_CARD:
      return 0;
  }

  return NaN;
}

function compareByType(hand1: Hand, hand2: Hand): number {
  const rank1 = calculateRank(hand1);
  const rank2 = calculateRank(hand2);
  return rank1 > rank2 ? 1 : rank1 < rank2 ? -1 : 0;
}

function compareLexicographically(hand1: Hand, hand2: Hand): number {
  for (let i = 0; i < 5; i++) {
    const rank1 = CARD_RANKS[hand1.cards[i]];
    const rank2 = CARD_RANKS[hand2.cards[i]];

    if (rank1 < rank2) {
      return -1;
    } else if (rank1 > rank2) {
      return 1;
    }
  }

  return 0;
}

const hands: Array<Hand> = [];

rl.on("line", (line) => {
  const [cards, bid] = line.trim().split(" ");
  hands.push({
    cards: cards.split(""),
    bid: parseInt(bid),
  });
});

rl.on("close", () => {
  hands.sort((hand1, hand2) => {
    const typesComparison = compareByType(hand1, hand2);
    return typesComparison == 0
      ? compareLexicographically(hand1, hand2)
      : typesComparison;
  });

  const result = hands.reduce(
    (acc: number, hand: Hand, rank: number) => acc + hand.bid * (rank + 1),
    0,
  );
  console.log(result);
});
