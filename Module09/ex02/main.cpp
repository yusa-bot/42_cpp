#include "PmergeMe.hpp"

#include <exception>
#include <iostream>

int main(int argc, char **argv) {
    try {
        PmergeMe sorter;
        sorter.process(argc, argv);
        sorter.display();
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << std::endl;
        return 1;
    }
    return 0;
}

// - 一般的な関数ではなく、コンテナ専用を実装すべし
// - 2つのcontainerの処理の違いは?
// - time表示
// - 重複処理
