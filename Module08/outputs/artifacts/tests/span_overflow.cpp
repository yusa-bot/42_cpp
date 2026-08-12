#include "Span.hpp"
#include <climits>
#include <iostream>
#include <limits>
int main() {
    Span s(3);
    s.addNumber(INT_MIN);
    s.addNumber(0);
    s.addNumber(INT_MAX);
    const unsigned int expected = std::numeric_limits<unsigned int>::max();
    const unsigned int longest = s.longestSpan();
    const unsigned int shortest = s.shortestSpan();
    std::cout << "longest=" << longest << " expected=" << expected << std::endl;
    std::cout << "shortest=" << shortest << " expected=" << static_cast<unsigned int>(INT_MAX) << std::endl;
    if (longest != expected || shortest != static_cast<unsigned int>(INT_MAX))
        return 1;
    Span one(1); one.addNumber(5);
    try { one.shortestSpan(); return 2; }
    catch (const std::exception& e) { std::cout << "one=" << e.what() << std::endl; }
    Span cap(2); int arr[] = {1,2,3};
    try { cap.addNumber(arr, arr+3); }
    catch (const std::exception& e) { std::cout << "range=" << e.what() << std::endl; }
    cap.addNumber(10); cap.addNumber(20);
    std::cout << "recovery=" << cap.shortestSpan() << std::endl;
    return cap.shortestSpan() == 10 ? 0 : 3;
}
