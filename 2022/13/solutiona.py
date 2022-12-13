import json
import sys


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
    total = 0

    for i, (prev, curr) in enumerate(pairs(sys.stdin)):
        if compare(prev, curr) < 0:
            total += i + 1

    print(total)


if __name__ == "__main__":
    main()
