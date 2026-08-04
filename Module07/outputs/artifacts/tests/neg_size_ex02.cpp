#include "Array.hpp"
#include <iostream>
#include <climits>
int main() {
    std::cout << "(-1) as unsigned int = " << static_cast<unsigned int>(-1) << std::endl;
    // -1 は unsigned int に暗黙変換され UINT_MAX になる
    try {
        Array<int> a(static_cast<unsigned int>(-1));   // 巨大サイズ確保を試みる
        std::cout << "constructed, size=" << a.size() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "caught std::exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "caught non-std exception" << std::endl;
    }
    std::cout << "survived (no crash / no UB)" << std::endl;
    return 0;
}
