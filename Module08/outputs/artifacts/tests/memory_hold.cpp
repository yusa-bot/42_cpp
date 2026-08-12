#include "easyfind.hpp"
#include "Span.hpp"
#include "MutantStack.hpp"
#include <list>
#include <unistd.h>
#include <vector>

int main() {
    std::vector<int> values(10000, 7);
    if (*easyfind(values, 7) != 7)
        return 1;

    Span span(10000);
    span.addNumber(values.begin(), values.end());
    if (span.shortestSpan() != 0 || span.longestSpan() != 0)
        return 2;

    MutantStack<int, std::list<int> > stack;
    stack.push(1);
    stack.push(2);
    if (stack.begin() == stack.end())
        return 3;

    sleep(8);
    return 0;
}
