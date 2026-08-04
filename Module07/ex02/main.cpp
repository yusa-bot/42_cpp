#include <iostream>
#include <string>

#include "Array.hpp"

int main() {

    // ---------- int ----------
    Array<int> empty;
    Array<int> numbers(3);

    std::cout << "empty size: " << empty.size() << std::endl;
    std::cout << "default value: " << numbers[0] << std::endl;
    numbers[0] = 42;

    // operator=
    try {
        Array<int> copy(numbers);
        const Array<int> readOnly(numbers);

        Array<int> assigned;
        assigned = numbers;

        numbers[0] = 0;

        std::cout << "copy: " << copy[0] << std::endl;
        std::cout << "assigned: " << assigned[0] << std::endl;

        // const
        std::cout << "const access: " << readOnly[0] << std::endl;
        // const Array なので書き換え不可
        // readOnly[0] = 1;

    } catch (const std::exception& exception) {
        std::cout << "exception: " << exception.what() << std::endl;
    }

    // out_of_range
    try {
        std::cout << numbers[3] << std::endl;
    } catch (const std::exception& exception) {
        // std::out_of_range() は std::exception 派生
        std::cout << "exception: " << exception.what() << std::endl;
    }

    // ---------- std::string ----------
    Array<std::string> words(2);
    words[0] = "hello";
    words[1] = "array";
    std::cout << words[0] << " " << words[1] << std::endl;

    return 0;
}
