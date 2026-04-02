#include "ClapTrap.hpp"
#include <iostream>

int main() {
    std::cout << "--- コンストラクタのテスト ---" << std::endl;
    ClapTrap a("Alpha");
    ClapTrap b("Beta");
    ClapTrap c; // デフォルトコンストラクタ
    c = a;      // コピー代入演算子
    ClapTrap d(b); // コピーコンストラクタ

    std::cout << "\n--- 基本アクションのテスト ---" << std::endl;
    // AlphaがBetaを攻撃（デフォルトの攻撃力は0）
    a.attack("Beta");
    b.takeDamage(0);

    // Betaが回復
    b.beRepaired(5);

    std::cout << "\n--- EP（エネルギーポイント）枯渇テスト ---" << std::endl;
    // Alphaの初期EPは10。すでに1回行動しているため残り9。
    // 9回行動させてEPを0にする。
    for (int i = 0; i < 9; i++) {
        a.attack("Dummy");
    }

    // EPが0の状態で行動を試みる（失敗するべき）
    a.attack("Beta");
    a.beRepaired(10);

    std::cout << "\n--- HP（ヒットポイント）枯渇テスト ---" << std::endl;
    // Betaに致命傷を与える（現在のHP15を超えるダメージ）
    b.takeDamage(20);

    // HPが0の状態でさらにダメージを受ける
    b.takeDamage(5);

    // HPが0の状態で行動を試みる（失敗するべき）
    b.attack("Alpha");
    b.beRepaired(10);

    std::cout << "\n--- デストラクタのテスト ---" << std::endl;
    return 0;
}
