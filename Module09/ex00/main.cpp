#include <iostream>

#include "BitcoinExchange.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: ./btc <input_file>" << std::endl;
        return 1;
    }

    try {
        BitcoinExchange exchange;
        exchange.processFile(argv[1]);
    // ファイルを開けない・DBが壊れているといった致命的エラーのみthrow
    } catch (const std::exception& exception) {
        std::cout << "Error: " << exception.what() << "." << std::endl;
        return 1;
    }
    return 0;
}
