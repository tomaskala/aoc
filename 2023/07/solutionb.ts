import { stdin } from "process";
import { createInterface } from "readline";

const rl = createInterface({
  input: stdin,
});

type Hand = {
  cards: string;
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
  T: 9,
  "9": 8,
  "8": 7,
  "7": 6,
  "6": 5,
  "5": 4,
  "4": 3,
  "3": 2,
  "2": 1,
  J: 0,
};

function countCards(cards: string): Map<string, number> {
  return Array.from(cards).reduce(
    (acc, card) => acc.set(card, (acc.get(card) || 0) + 1),
    new Map<string, number>(),
  );
}

function calculateRank(hand: Hand): number {
  let cardCounts;

  if (hand.cards === "JJJJJ") {
    cardCounts = countCards(hand.cards);
  } else {
    cardCounts = countCards(hand.cards.replace(new RegExp("J", "g"), ""));
    const mostCommon = Array.from(cardCounts.entries()).reduce((acc, x) =>
      x[1] > acc[1] ? x : acc,
    );

    const realCards = hand.cards.replace(new RegExp("J", "g"), mostCommon[0]);
    cardCounts = countCards(realCards);
  }

  const handType = Array.from(cardCounts.values());
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
    cards: cards,
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
