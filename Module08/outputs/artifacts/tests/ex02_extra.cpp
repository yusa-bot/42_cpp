#include "MutantStack.hpp"
#include <iostream>
int main() {
    // 空スタックの begin()==end()（クラッシュしない）
    MutantStack<int> empty;
    std::cout << "empty begin==end: " << (empty.begin() == empty.end()) << std::endl;

    // コピーの独立性
    MutantStack<int> a; a.push(1); a.push(2);
    MutantStack<int> b(a);
    b.push(99);
    std::cout << "a.size=" << a.size() << " b.size=" << b.size() << " (expect 2/3)" << std::endl;

    // 代入の独立性
    MutantStack<int> c; c = a;
    c.push(7);
    std::cout << "a.size=" << a.size() << " c.size=" << c.size() << " (expect 2/3)" << std::endl;
    return 0;
}
