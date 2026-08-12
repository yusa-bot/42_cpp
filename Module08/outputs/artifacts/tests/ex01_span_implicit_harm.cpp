// Purpose: ex01 Span(unsigned int) に explicit が無いことの「不具合」を実演する。
//   本来 Span を渡すべき関数 report(const Span&) に、誤って int(5) を渡したコードが、
//   explicit 非指定のため int -> Span への暗黙変換で「コンパイル成功」してしまい、
//   実行時に「空の一時 Span」に対する shortestSpan() が std::logic_error を投げて
//   terminate (abort) する = 型取り違えという明白な誤りがコンパイル時に検出されず
//   実行時クラッシュとして潜伏する、という不具合を示す。
//
// Build & Run (ワークスペースルート = /Users/fujiki/workspace/42tokyo/cpp-module):
//   c++ -Wall -Wextra -Werror -std=c++98 -I cpp-module--ayusa/Module08/ex01 \
//     cpp-module--ayusa/Module08/outputs/artifacts/tests/ex01_span_implicit_harm.cpp \
//     cpp-module--ayusa/Module08/ex01/Span.cpp -o ex01_span_implicit_harm \
//   && /tmp/ex01_span_implicit_harm
//
// 観測 (explicit なし = 現状):
//   - コンパイル成功 (暗黙変換 int -> Span(5) が通る)
//   - 実行時: "report: shortest=" 出力後、
//     libc++abi: terminating due to uncaught exception of type std::logic_error:
//       A span requires at least two numbers
//     で abort (終了コード 134)
//
// 期待される正しい挙動 (もし explicit Span(unsigned int) を指定していれば):
//   - report(5) は "error: no viable conversion from 'int' to 'Span'" で
//     コンパイル時に弾かれ、実行時破綻は起こらない。
//
// 補足: 単純形 `Span s = 5;` も explicit なしでは同様にコンパイル成功する
//       (意図しない空 Span 生成)。

#include "Span.hpp"
#include <iostream>

// Span を受け取るつもりの関数
void report(const Span& s)
{
    std::cout << "report: shortest=" << s.shortestSpan() << "\n"; // 空 Span なら例外
}

int main()
{
    // (1) 単純形: explicit なしなら暗黙変換で通ってしまう (意図しない空 Span)
    Span s = 5;    // explicit があればここでコンパイルエラー
    (void)s;

    // (2) 本命: int を誤って渡した関数呼び出しが暗黙変換で通り、実行時に破綻
    std::cout << "call report(5) ...\n";
    report(5);     // explicit があればここでコンパイルエラー
    std::cout << "done\n";
    return 0;
}
