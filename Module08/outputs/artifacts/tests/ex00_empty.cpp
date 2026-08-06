#include "easyfind.hpp"
#include <vector>
#include <iostream>
int main() {
    std::vector<int> empty;
    try { easyfind(empty, 1); }
    catch (const std::exception& e) { std::cout << "empty container: " << e.what() << std::endl; }
    return 0;
}
