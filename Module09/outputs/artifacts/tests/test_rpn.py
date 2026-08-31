#!/usr/bin/env python3
import pathlib
import subprocess
import sys


def invoke(binary, expression=None):
    command = [str(binary)]
    if expression is not None:
        command.append(expression)
    return subprocess.run(
        command, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        timeout=10
    )


def main():
    if len(sys.argv) != 2:
        print("usage: test_rpn.py BINARY", file=sys.stderr)
        return 2
    binary = pathlib.Path(sys.argv[1]).resolve()
    failures = []
    valid = {
        "8 9 * 9 - 9 - 9 - 4 - 1 +": "42\n",
        "9 8 * 4 * 4 / 2 + 9 - 8 - 8 - 1 - 6 -": "42\n",
        "1 2 * 2 / 2 + 5 * 6 - 1 3 * - 4 5 * * 8 /": "15\n",
        "1 2 * 2 / 2 * 2 4 - +": "0\n",
        "9": "9\n",
        "7 8 -": "-1\n",
    }
    for expression, expected in valid.items():
        result = invoke(binary, expression)
        if result.returncode != 0 or result.stdout != expected or result.stderr != "":
            failures.append("valid expression failed: " + expression)

    invalid = [None, "", "1 0 /", "1 +", "1 2", "(1 + 1)", "10", "1.5"]
    for expression in invalid:
        result = invoke(binary, expression)
        label = "<no argument>" if expression is None else repr(expression)
        if result.returncode == 0:
            failures.append("invalid expression returned success: " + label)
        if result.stderr == "":
            failures.append("error was not written to stderr: " + label)
        if result.stdout != "":
            failures.append("error polluted stdout: " + label)

    if failures:
        for failure in failures:
            print("FAIL:", failure)
        return 1
    print("PASS: RPN arithmetic and stderr error-channel tests")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
