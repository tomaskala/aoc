import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

class Solution {
  static class Pair {
    int fst;
    int snd;

    Pair(int fst, int snd) {
      this.fst = fst;
      this.snd = snd;
    }
  }

  public static void main(String[] args) {
    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
    ArrayList<Integer> numbers = br.lines()
      .map(Integer::parseInt)
      .collect(Collectors.toCollection(ArrayList::new));
    ArrayList<Pair> numberPairs = IntStream.range(0, numbers.size())
      .mapToObj(i -> new Pair(i, numbers.get(i)))
      .collect(Collectors.toCollection(ArrayList::new));

    for (Pair pair : new ArrayList<>(numberPairs)) {
      int oldIndex = numberPairs.indexOf(pair);
      numberPairs.remove(oldIndex);

      int newIndex = Math.floorMod(oldIndex + pair.snd, numberPairs.size());
      numberPairs.add(newIndex, pair);
    }

    int zeroPos = IntStream.range(0, numberPairs.size())
      .filter(i -> numberPairs.get(i).snd == 0)
      .findFirst()
      .getAsInt();

    int result = numberPairs.get((zeroPos + 1000) % numberPairs.size()).snd
               + numberPairs.get((zeroPos + 2000) % numberPairs.size()).snd
               + numberPairs.get((zeroPos + 3000) % numberPairs.size()).snd;

    System.out.println(result);
  }
}
