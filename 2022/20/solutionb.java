import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

class Solution {
  static final long key = 811589153;

  static class Pair {
    long fst;
    long snd;

    Pair(long fst, long snd) {
      this.fst = fst;
      this.snd = snd;
    }
  }

  public static void main(String[] args) {
    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
    ArrayList<Long> numbers = br.lines()
      .map(Long::parseLong)
      .map(i -> i * key)
      .collect(Collectors.toCollection(ArrayList::new));
    ArrayList<Pair> numberPairs = IntStream.range(0, numbers.size())
      .mapToObj(i -> new Pair(i, numbers.get(i)))
      .collect(Collectors.toCollection(ArrayList::new));

    List<Pair> original = new ArrayList<>(numberPairs);

    for (int i = 0; i < 10; i++) {
      for (Pair pair : original) {
        int oldIndex = numberPairs.indexOf(pair);
        numberPairs.remove(oldIndex);

        int newIndex = Math.floorMod(oldIndex + pair.snd, numberPairs.size());
        numberPairs.add(newIndex, pair);
      }
    }

    int zeroPos = IntStream.range(0, numberPairs.size())
      .filter(i -> numberPairs.get(i).snd == 0)
      .findFirst()
      .getAsInt();

    long result = numberPairs.get((zeroPos + 1000) % numberPairs.size()).snd
               + numberPairs.get((zeroPos + 2000) % numberPairs.size()).snd
               + numberPairs.get((zeroPos + 3000) % numberPairs.size()).snd;

    System.out.println(result);
  }
}
