#include "Array.hpp"
#include <iostream>
struct Big { char pad[65536]; };   // 64KB/要素 → UINT_MAX 個で天文学的サイズ → 確保失敗を狙う
int main() {
    try {
        Array<Big> a(static_cast<unsigned int>(-1));   // 事実上確保不可能
        std::cout << "constructed size=" << a.size() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "caught std::exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "caught non-std" << std::endl;
    }
    std::cout << "no segfault / no UB" << std::endl;
    return 0;
}
