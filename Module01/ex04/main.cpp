#include "FileReplacer.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cout << "Usage: ./replace <filename> <s1> <s2>" << std::endl;
        return 1;
    }

    FileReplacer replacer(argv[1], argv[2], argv[3]);
    return replacer.execute();
}
