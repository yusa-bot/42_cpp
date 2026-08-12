// Purpose: ayusa 版 easyfind (const overload あり) に const コンテナを渡すと
//          コンパイル・実行できることを示す検証コード。
//
// 期待結果: コンパイル成功・実行成功
//   出力: compiled: const container OK
//   （const コンテナには easyfind(const T&, int) が選ばれ const_iterator を返す）
//
// Build (ワークスペースルートから実行):
//   c++ -std=c++98 -Wall -Wextra -Werror \
//     -Icpp-module--ayusa/Module08/ex00 \
//     cpp-module--ayusa/Module08/outputs/artifacts/tests/ex00_const_container_ayusa.cpp \
//     -o /tmp/ex00_const_container_ayusa && /tmp/ex00_const_container_ayusa

#include "easyfind.hpp"
#include <vector>
#include <iostream>

int main()
{
    const std::vector<int> cv(3, 5);   // const コンテナ
    easyfind(cv, 5);                    // ayusa 版は const overload が選ばれ OK
    std::cout << "compiled: const container OK\n";
    return 0;
}
