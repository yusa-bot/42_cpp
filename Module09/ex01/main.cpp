J#include <iostream>

#include "RPN.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: ./RPN \"expression\"" << std::endl;
        return 1;
    }

    try {
        RPN calculator;
        std::cout << calculator.evaluate(argv[1]) << std::endl;
    } catch (const std::exception& exception) {
        std::cout << "Error: " << exception.what() << std::endl;
        return 1;
    }
    return 0;
}
