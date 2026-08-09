#!/bin/sh
# Reproducible ScalarConverter test battery (run from ex00 after `make`).
# Usage: cd cpp-module--ayusa/Module06/ex00 && make && sh ../outputs/artifacts/tests/scalar_cases.sh
for a in 0 nan 42.0f "'c'" "'a'" -42 42 +inf -inf +inff -inff nanf 2147483648 -2147483649 42.0 0.0f 126 127 128 abc "" 42abc 3.14 "'0'" 1e40 -1e40; do
    echo "===== ./convert [$a] ====="
    ./convert "$a"
done
