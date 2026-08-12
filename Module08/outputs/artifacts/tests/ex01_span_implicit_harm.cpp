// Purpose: ex01 Span(unsigned int) が explicit であり、int -> Span の意図しない
//          暗黙変換がコンパイル時に拒否されることを確認する負のテスト。
//   本来 Span を渡すべき関数 report(const Span&) に int(5) を渡すコードと
//   `Span s = 5;` の双方がコンパイルエラーになることを期待する。
//
// Build & Run (ワークスペースルート = /Users/fujiki/workspace/42tokyo/cpp-module):
//   c++ -Wall -Wextra -Werror -std=c++98 -I cpp-module--ayusa/Module08/ex01 \
//     cpp-module--ayusa/Module08/outputs/artifacts/tests/ex01_span_implicit_harm.cpp \
//     cpp-module--ayusa/Module08/ex01/Span.cpp -o ex01_span_implicit_harm \
//
// Expected: コンパイル失敗
//   - `Span s = 5;` は no viable conversion
//   - `report(5)` は no matching function
//
#include "Span.hpp"
#include <iostream>

// Span を受け取るつもりの関数
void report(const Span& s)
{
    std::cout << "report: shortest=" << s.shortestSpan() << "\n"; // 空 Span なら例外
}

int main()
{
    // (1) copy-initializationによるint -> Span変換は拒否されるべき
    Span s = 5;
    (void)s;

    // (2) 関数引数でのint -> Span変換も拒否されるべき
    std::cout << "call report(5) ...\n";
    report(5);
    std::cout << "done\n";
    return 0;
}
