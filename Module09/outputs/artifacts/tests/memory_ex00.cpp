#include "BitcoinExchange.hpp"

#include <unistd.h>

int main() {
    {
        BitcoinExchange first;
        BitcoinExchange second(first);
        BitcoinExchange third;
        third = second;
        first.processFile("input.txt");
    }
    sleep(4);
    return 0;
}
