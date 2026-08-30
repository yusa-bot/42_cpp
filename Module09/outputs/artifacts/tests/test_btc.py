#!/usr/bin/env python3
import pathlib
import subprocess
import sys


def run(binary, cwd, *args):
    return subprocess.run(
        [str(binary), *args], cwd=str(cwd), text=True,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=10
    )


def require(condition, message, failures):
    if not condition:
        failures.append(message)


def main():
    if len(sys.argv) != 4:
        print("usage: test_btc.py BINARY EX00_DIR TMP_DIR", file=sys.stderr)
        return 2
    binary = pathlib.Path(sys.argv[1]).resolve()
    ex00 = pathlib.Path(sys.argv[2]).resolve()
    tmp = pathlib.Path(sys.argv[3]).resolve()
    tmp.mkdir(parents=True, exist_ok=True)
    failures = []

    boundary = tmp / "btc_boundary_input.txt"
    boundary.write_text(
        "date | value\n"
        "2011-01-03 | 3\n"
        "2011-01-04 | 2\n"
        "2009-01-01 | 1\n"
        "2012-02-29 | 1\n"
        "1900-02-29 | 1\n"
        "2000-02-29 | 1\n"
        "2022-03-29 | 0\n"
        "2022-03-29 | 1000\n"
        "2022-03-29 | 1000.000\n"
        "2022-03-29 | 1000.001\n"
        "2022-03-29 | -0.001\n"
        "2022-03-29 | nan\n"
        "2022-03-29 | inf\n"
        "bad line\n"
        "2011-01-04 | 2\n",
        encoding="utf-8",
    )
    result = run(binary, ex00, str(boundary))
    require(result.returncode == 0, "boundary file returned nonzero", failures)
    require("2011-01-03 => 3 = 0.9" in result.stdout,
            "nearest lower date calculation failed", failures)
    require("2012-02-29 => 1 = 4.98" in result.stdout,
            "valid leap-day lower-date calculation failed", failures)
    require(result.stdout.count("2011-01-04 => 2 = 0.6") == 2,
            "processing did not continue through all erroneous lines", failures)
    require("Error: too large a number." in result.stdout,
            "value above 1000 was not rejected", failures)
    require("Error: not a positive number." in result.stdout,
            "negative value was not rejected", failures)
    require("Error: no exchange rate available for date." in result.stdout,
            "date before database start was not rejected", failures)
    require("Error: bad input => 1900-02-29 | 1" in result.stdout,
            "invalid calendar date was not rejected", failures)
    require("Error: bad input => bad line" in result.stdout,
            "malformed line was not rejected", failures)

    empty = tmp / "btc_empty.txt"
    empty.write_text("", encoding="utf-8")
    result = run(binary, ex00, str(empty))
    require(result.returncode == 0 and result.stdout == "" and result.stderr == "",
            "empty input file was not handled cleanly", failures)

    result = run(binary, ex00, str(tmp / "does_not_exist.txt"))
    require(result.returncode != 0 and "Error:" in result.stdout,
            "missing input file was not rejected", failures)

    no_header = tmp / "btc_no_header.txt"
    no_header.write_text("2011-01-03 | 3\n", encoding="utf-8")
    result = run(binary, ex00, str(no_header))
    print("INFO: no-header exit=%d stdout=%r stderr=%r" %
          (result.returncode, result.stdout, result.stderr))

    result = run(binary, ex00)
    require(result.returncode != 0 and "Usage:" in result.stdout,
            "missing argument was not rejected", failures)

    if failures:
        for failure in failures:
            print("FAIL:", failure)
        return 1
    print("PASS: btc boundary/error/continuation tests")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
