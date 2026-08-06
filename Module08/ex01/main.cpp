#include <exception>
#include <iostream>
#include <vector>

#include "Span.hpp"

int main() {
    Span sample(5);
    sample.addNumber(6);
    sample.addNumber(3);
    sample.addNumber(17);
    sample.addNumber(9);
    sample.addNumber(11);

    std::cout << "shortest: " << sample.shortestSpan() << std::endl;
    std::cout << "longest: " << sample.longestSpan() << std::endl;

    try {
        sample.addNumber(42);
    } catch (const std::exception& exception) {
        std::cout << "full: " << exception.what() << std::endl;
    }

    Span empty(0);
    try {
        empty.shortestSpan();
    } catch (const std::exception& exception) {
        std::cout << "empty: " << exception.what() << std::endl;
    }

	// ----------- container -----------
    std::vector<int> bulk;
    for (int value = 0; value < 10000; ++value)
        bulk.push_back(value * 3);

    Span large(10000);
    large.addNumber(bulk.begin(), bulk.end());
    std::cout << "10000 numbers shortest: " << large.shortestSpan()
              << std::endl;
    std::cout << "10000 numbers longest: " << large.longestSpan()
              << std::endl;
    return 0;
}
