// Purpose: fugu 版 easyfind (単一 overload, 非const T& のみ) に const コンテナを渡すと
//          コンパイルエラーになることを示す検証コード。
//
// 期待結果: コンパイル失敗
//   error: no viable conversion from '__wrap_iter<const int *>' to '__wrap_iter<pointer>'
//   （const コンテナの begin() が返す const_iterator を T::iterator に代入できないため）
//
// Build (ワークスペースルート = /Users/fujiki/workspace/42tokyo/cpp-module から実行可):
//   c++ -std=c++98 -Wall -Wextra -Werror \
//     cpp-module--ayusa/Module08/outputs/artifacts/tests/ex00_const_container_fugu.cpp \
//     -o /tmp/ex00_const_container_fugu
//   （↑ 非ゼロ終了＝コンパイル失敗が期待挙動）

#include "/Users/fujiki/workspace/42tokyo/cpp-module/cpp-module--fugu/08/ex00/easyfind.hpp"
#include <vector>
#include <iostream>

int main()
{
    const std::vector<int> cv(3, 5);   // const コンテナ
    easyfind(cv, 5);                    // fugu 版は const T& を受けられずコンパイルエラー
    std::cout << "compiled: const container OK\n";
    return 0;
}
