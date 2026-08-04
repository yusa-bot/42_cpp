#include "Array.hpp"
#include <iostream>
#include <string>

int main() {
    Array<int> a(3);
    a[0]=10; a[1]=20; a[2]=30;

    // 深いコピーの独立性 (copy ctor)
    Array<int> b(a);
    b[0]=999;
    std::cout << "deep-copy independent: a[0]=" << a[0] << " b[0]=" << b[0] << " (expect 10/999)\n";

    // operator= の独立性
    Array<int> c;
    c = a;
    c[1]=777;
    std::cout << "assign independent: a[1]=" << a[1] << " c[1]=" << c[1] << " (expect 20/777)\n";

    // 自己代入
    Array<int>& aref = a; a = aref;
    std::cout << "self-assign ok: a[0]=" << a[0] << " size=" << a.size() << "\n";

    // 空配列への添字アクセス (0要素) は例外
    Array<int> empty;
    try { std::cout << empty[0] << "\n"; }
    catch (const std::exception& e) { std::cout << "empty[0] throws: " << e.what() << "\n"; }

    // 境界: size()==3, index 2 ok, 3 throws
    try { std::cout << "a[2]=" << a[2] << "\n"; a[3]=1; }
    catch (const std::exception& e) { std::cout << "a[3] throws: " << e.what() << "\n"; }

    // サイズ不一致の代入 (3 <- 空) でサイズが更新されるか
    Array<int> big(5);
    big = empty;
    std::cout << "assign shrink: big.size()=" << big.size() << " (expect 0)\n";
    return 0;
}
