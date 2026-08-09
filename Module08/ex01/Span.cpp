#include "Span.hpp"

#include <algorithm>

Span::Span() : _capacity(0), _numbers() {
}

Span::Span(unsigned int capacity) : _capacity(capacity), _numbers() {
    _numbers.reserve(capacity);
}

Span::Span(const Span& other)
    : _capacity(other._capacity), _numbers(other._numbers) {
}

Span& Span::operator=(const Span& other) {
    if (this != &other) {
        std::vector<int> copied(other._numbers);

        _numbers.swap(copied);
        _capacity = other._capacity;
    }
    return *this;
}

Span::~Span() {
}

unsigned int Span::distanceBetween(int first, int second) {
    if (first > second) {
        const int temporary = first;
        first = second;
        second = temporary;
    }
    if (first < 0 && second >= 0) {
        const unsigned int negativePart =
            static_cast<unsigned int>(-(first + 1)) + 1U; // firstがINT_MINの場合を考慮
        const unsigned int distance =
            static_cast<unsigned int>(second) + negativePart;
        return distance;
    }
    return static_cast<unsigned int>(second - first);
}

void Span::addNumber(int number) {
    if (_numbers.size() >= static_cast<std::size_t>(_capacity))
        throw std::length_error("Span capacity exceeded");
    _numbers.push_back(number);
}

unsigned int Span::shortestSpan() const {
    if (_numbers.size() < 2)
        throw std::logic_error("A span requires at least two numbers");

    std::vector<int> sorted(_numbers);
    std::sort(sorted.begin(), sorted.end());

    unsigned int shortest = distanceBetween(sorted[0], sorted[1]);
    for (std::size_t index = 1; index < sorted.size() - 1; ++index) {
        const unsigned int distance =
            distanceBetween(sorted[index], sorted[index + 1]);
        if (distance < shortest)
            shortest = distance;
    }
    return static_cast<unsigned int>(shortest);
}

unsigned int Span::longestSpan() const {
    if (_numbers.size() < 2)
        throw std::logic_error("A span requires at least two numbers");

    const std::vector<int>::const_iterator minimum =
        std::min_element(_numbers.begin(), _numbers.end());
    const std::vector<int>::const_iterator maximum =
        std::max_element(_numbers.begin(), _numbers.end());
    return distanceBetween(*maximum, *minimum);
}
