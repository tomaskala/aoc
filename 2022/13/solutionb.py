import json
import sys
from functools import cmp_to_key, reduce
from operator import mul


def pairs(f):
    prev = curr = None

    for line in map(str.strip, f):
        if not line:
            yield prev, curr
        else:
            prev = curr
            curr = json.loads(line)

    yield prev, curr


def compare(left, right):
    if isinstance(left, int) and isinstance(right, int):
        return left - right

    if isinstance(left, int):
        left = [left]

    if isinstance(right, int):
        right = [right]

    return next(filter(lambda c: c, map(compare, left, right)), len(left) - len(right))


def main():
    dividers = [[[2]], [[6]]]
    packets = list(map(json.loads, filter(lambda l: l, map(str.strip, sys.stdin))))
    packets.extend(dividers)

    packets.sort(key=cmp_to_key(compare))

    key = reduce(mul, map(lambda d: packets.index(d) + 1, dividers))
    print(key)


if __name__ == "__main__":
    main()
