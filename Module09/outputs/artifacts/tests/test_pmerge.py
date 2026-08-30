#!/usr/bin/env python3
import pathlib
import random
import re
import subprocess
import sys


def run(binary, values):
    return subprocess.run(
        [str(binary), *[str(value) for value in values]], text=True,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=30
    )


def check_sorted_run(binary, values, failures, label):
    result = run(binary, values)
    if result.returncode != 0:
        failures.append(label + ": nonzero exit: " + result.stderr.strip())
        return
    lines = result.stdout.splitlines()
    if len(lines) != 4:
        failures.append(label + ": expected exactly four output lines")
        return
    if not lines[0].startswith("Before:") or not lines[1].startswith("After:"):
        failures.append(label + ": missing Before/After labels")
        return
    try:
        observed = [int(token) for token in lines[1][len("After:"):].split()]
    except ValueError:
        failures.append(label + ": After line contains a non-integer")
        return
    if observed != sorted(values):
        failures.append(label + ": After sequence differs from Python sorted oracle")
    if "std::vector" not in lines[2] or "std::deque" not in lines[3]:
        failures.append(label + ": timing lines do not identify both containers")
    vector_match = re.search(r"std::vector:\s+([0-9]+(?:\.[0-9]+)?) us$", lines[2])
    deque_match = re.search(r"std::deque:\s+([0-9]+(?:\.[0-9]+)?) us$", lines[3])
    if vector_match is None or deque_match is None:
        failures.append(label + ": timing values are not parseable microseconds")
    elif label == "descending-3000-unique":
        vector_time = float(vector_match.group(1))
        deque_time = float(deque_match.group(1))
        if vector_time <= 0.0 or deque_time <= 0.0:
            failures.append(label + ": timing precision collapsed to zero")
        print("INFO: 3000 timing vector=%.5f us deque=%.5f us" %
              (vector_time, deque_time))


def main():
    if len(sys.argv) != 2:
        print("usage: test_pmerge.py BINARY", file=sys.stderr)
        return 2
    binary = pathlib.Path(sys.argv[1]).resolve()
    failures = []

    fixed = [
        [1], [2, 1], [3, 5, 9, 7, 4], list(range(20, 0, -1)),
        list(range(1, 21)), [5, 1, 5, 3, 3, 2], [2147483647, 1, 2],
    ]
    for index, values in enumerate(fixed):
        check_sorted_run(binary, values, failures, "fixed-%d" % index)

    generator = random.Random(4209)
    for size in range(1, 65):
        for trial in range(3):
            values = generator.sample(range(1, 1000000), size)
            check_sorted_run(binary, values, failures,
                             "random-%d-%d" % (size, trial))

    check_sorted_run(binary, list(range(3000, 0, -1)), failures,
                     "descending-3000-unique")

    invalid = [[], [0], [-1, 2], [2147483648], ["+1"], ["1.0"], [""]]
    for values in invalid:
        result = run(binary, values)
        if result.returncode == 0 or result.stderr == "":
            failures.append("invalid input not rejected on stderr: %r" % (values,))

    if failures:
        for failure in failures:
            print("FAIL:", failure)
        return 1
    print("PASS: 200 oracle runs, including 3000 unique integers and invalid inputs")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
