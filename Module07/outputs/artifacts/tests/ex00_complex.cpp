#include "whatever.hpp"
#include <iostream>
#include <string>

// 複合型: 比較演算子をすべて備えたクラス
class Val {
public:
    int v; int id;
    Val(int v_, int id_) : v(v_), id(id_) {}
    bool operator<(const Val& o) const { return v < o.v; }
    bool operator>(const Val& o) const { return v > o.v; }
};

int main() {
    // swap on complex type
    Val a(1, 100), b(2, 200);
    ::swap(a, b);
    std::cout << "after swap: a.v=" << a.v << " b.v=" << b.v << std::endl; // 2,1

    // equal values: min/max must return the SECOND one (subject spec)
    Val e1(5, 1), e2(5, 2);
    std::cout << "min equal -> id " << ::min(e1, e2).id << " (expect 2)" << std::endl;
    std::cout << "max equal -> id " << ::max(e1, e2).id << " (expect 2)" << std::endl;

    // string complex type
    std::string s1 = "abc", s2 = "abd";
    std::cout << "min(str)=" << ::min(s1, s2) << " max(str)=" << ::max(s1, s2) << std::endl;
    return 0;
}
