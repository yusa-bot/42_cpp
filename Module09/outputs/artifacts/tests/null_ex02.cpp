#include "PmergeMe.hpp"

#include <exception>
#include <iostream>

int main() {
    char program[] = "PmergeMe";
    char *arguments[] = {program, 0};
    try {
        PmergeMe sorter;
        sorter.process(2, arguments);
    } catch (const std::exception& error) {
        std::cout << "caught: " << error.what() << std::endl;
        return 0;
    }
    return 1;
}
