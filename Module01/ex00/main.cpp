#include "Zombie.hpp"
#include <iostream>

int main() {

    std::cout << "--- Testing Heap Allocation with newZombie() ---" << std::endl;
    Zombie* heapZombie = newZombie("HeapZombie");
    heapZombie->announce();
    delete heapZombie; // デストラクタを呼ぶ -> メモリ解放

    std::cout << "\n--- Testing Stack Allocation with randomChump()) ---" << std::endl;
    randomChump("StackZombie");

    std::cout << "\n--- End of tests ---" << std::endl;
    return 0;
}
