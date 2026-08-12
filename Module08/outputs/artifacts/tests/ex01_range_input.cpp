#include "Span.hpp"
#include <sstream>
#include <iterator>
#include <iostream>

int main() {
    std::istringstream input("8 3 11 4");
    Span values(4);
    values.addNumber(std::istream_iterator<int>(input),
                     std::istream_iterator<int>());
    std::cout << "input shortest=" << values.shortestSpan()
              << " longest=" << values.longestSpan() << std::endl;

    std::istringstream tooMany("1 2 3");
    Span limited(2);
    try {
        limited.addNumber(std::istream_iterator<int>(tooMany),
                          std::istream_iterator<int>());
    } catch (const std::exception& exception) {
        std::cout << "overflow=" << exception.what() << std::endl;
    }
    limited.addNumber(10);
    limited.addNumber(20);
    std::cout << "recovery shortest=" << limited.shortestSpan() << std::endl;
    return 0;
}
