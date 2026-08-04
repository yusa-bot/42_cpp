#include <iostream>
#include <string>

#include "iter.hpp"

void increment(int& number) {
    ++number;
}

template <typename T>
void print(const T& value) {
    std::cout << value << std::endl;
}

int main() {
    int numbers[] = {1, 2, 3};
    const int readOnlyNumbers[] = {4, 5, 6};
    std::string words[] = {"template", "iter"};

    iter(numbers, 3, increment);
    iter(numbers, 3, print<int>);
    iter(readOnlyNumbers, 3, print<int>);
    iter(words, 2, print<std::string>);
    return 0;
}
