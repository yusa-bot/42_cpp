#include "Span.hpp"
#include <climits>
#include <iostream>
int main() {
    Span s(3);
    s.addNumber(INT_MIN);
    s.addNumber(0);
    s.addNumber(INT_MAX);
    std::cout << "longest (expect 4294967295): " << s.longestSpan() << std::endl;
    std::cout << "shortest (expect 2147483647): " << s.shortestSpan() << std::endl;

    Span one(1); one.addNumber(5);
    try { one.shortestSpan(); } catch (const std::exception& e) { std::cout << "one->shortest: " << e.what() << std::endl; }

    Span cap(2);
    int arr[] = {1,2,3};
    try { cap.addNumber(arr, arr+3); } catch (const std::exception& e) { std::cout << "range overflow: " << e.what() << std::endl; }
    cap.addNumber(10); cap.addNumber(20);
    std::cout << "cap shortest after recovery: " << cap.shortestSpan() << std::endl;
    return 0;
}
