#include <deque>
#include <iostream>
#include <list>
#include <vector>

#include "easyfind.hpp"

int main() {
    // ------------ vector ------------
    std::vector<int> numbers;
    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(10);

    try {
        std::vector<int>::iterator found = easyfind(numbers, 10);
        std::cout << "vector first match: " << *found << std::endl;
    } catch (const std::exception& exception) {
        std::cout << "exception: " << exception.what() << std::endl;
    }

    try {
        easyfind(numbers, 99);
    } catch (const std::exception& exception) {
        std::cout << "missing value: " << exception.what() << std::endl;
    }

    const std::vector<int> readOnlyNumbers(numbers); // std::vector<int> のコピーコンストラクタ
    try {
        std::vector<int>::const_iterator found = easyfind(readOnlyNumbers, 20);
        std::cout << "const vector match: " << *found << std::endl;
    } catch (const std::exception& exception) {
        std::cout << "exception: " << exception.what() << std::endl;
    }

    // ------------ list ------------
    std::list<int> values;
    values.push_back(3);
    values.push_back(6);
    try {
        std::cout << "list match: " << *easyfind(values, 6) << std::endl;
    } catch (const std::exception& exception) {
        std::cout << "exception: " << exception.what() << std::endl;
    }

    // ------------ deque(double-ended queue) ------------
    std::deque<int> queue;
    queue.push_back(7);
    queue.push_back(8);
    try {
        std::cout << "deque match: " << *easyfind(queue, 8) << std::endl;
    } catch (const std::exception& exception) {
        std::cout << "exception: " << exception.what() << std::endl;
    }
    return 0;
}
