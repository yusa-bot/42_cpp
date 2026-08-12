#ifndef SPAN_HPP
# define SPAN_HPP

# include <cstddef>
# include <stdexcept>
# include <vector>

// Key Point: <algorithm>

class Span {
private:
    unsigned int _capacity;
    std::vector<int> _numbers;

    static unsigned int distanceBetween(int first, int second);

public:
    Span();
    explicit Span(unsigned int capacity);
    Span(const Span& other);
    Span& operator=(const Span& other);
    ~Span();

    void addNumber(int number);

    template <typename InputIterator>
    void addNumber(InputIterator first, InputIterator last) {
        std::vector<int> pending(first, last);
        const std::size_t freeSlots =
            static_cast<std::size_t>(_capacity) - _numbers.size();

        if (pending.size() > freeSlots)
            throw std::length_error("Span capacity exceeded");
        _numbers.insert(_numbers.end(), pending.begin(), pending.end());
    }

    unsigned int shortestSpan() const;
    unsigned int longestSpan() const;
};

#endif
