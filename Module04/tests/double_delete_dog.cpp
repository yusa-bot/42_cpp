#include "../ex01/Dog.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <new>

namespace {
// 元の記述（C++11以降）
// std::atomic<bool> g_fail_next_allocation(false);

// C++98向け（単一スレッド前提）
bool g_fail_next_allocation = false;
}

void* operator new(std::size_t size) throw(std::bad_alloc) {
    // 元の記述（C++11以降）
    // if (g_fail_next_allocation.exchange(false)) {
    //     throw std::bad_alloc();
    // }
    if (g_fail_next_allocation) {
        g_fail_next_allocation = false;
        throw std::bad_alloc();
    }
    if (void* p = std::malloc(size)) {
        return p;
    }
    throw std::bad_alloc();
}

void operator delete(void* ptr) throw() {
    std::free(ptr);
}

int main() {
    std::cout << "[TEST] Reproduce double-delete risk in Dog::operator=\n";

    try {
        Dog dog1;
        dog1.setIdea(0, "source");

        Dog dog2;
        dog2.setIdea(0, "target");

        // Make the next allocation fail exactly inside `dog2 = dog1`.
        // 元の記述（C++11以降）
        // g_fail_next_allocation.store(true);
        g_fail_next_allocation = true;

        try {
            dog2 = dog1;
            std::cout << "[UNEXPECTED] assignment succeeded\n";
            return 1;
        } catch (const std::bad_alloc&) {
            std::cout << "[INFO] Caught expected std::bad_alloc during assignment\n";
        }

        std::cout << "[INFO] Leaving scope. If _brain is dangling, destructor may double-delete.\n";
    } catch (const std::exception& e) {
        std::cout << "[ERROR] Unexpected exception: " << e.what() << "\n";
        return 2;
    }

    std::cout << "[DONE] Program ended without runtime-detected issue\n";
    return 0;
}
