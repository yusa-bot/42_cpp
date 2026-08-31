#include "RPN.hpp"

#include <unistd.h>

int main() {
    {
        RPN first;
        first.evaluate("8 9 * 9 - 9 - 9 - 4 - 1 +");
        RPN second(first);
        RPN third;
        third = second;
        third.evaluate("1 2 +");
    }
    sleep(4);
    return 0;
}
