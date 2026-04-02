#include "FragTrap.hpp"
#include <iostream>

int main() {
    std::cout << "--- コンストラクタとOCFのテスト ---" << std::endl;
    FragTrap alice("Alice");
    FragTrap bob("Bob");
    std::cout << std::endl;

    FragTrap charlie(alice);
    FragTrap david;
    david = bob;

    std::cout << "\n--- 基本アクションのテスト ---" << std::endl;
    alice.attack("Bob");

    // HP100、EP100、攻撃力30が設定されているかの確認
    bob.takeDamage(30);
    bob.beRepaired(20);

    std::cout << "\n--- 特殊能力 (highFivesGuys) のテスト ---" << std::endl;
    alice.highFivesGuys();
    bob.highFivesGuys();

    std::cout << "\n--- 死亡時の動作テスト ---" << std::endl;
    // Bobに致命傷を与える（HP100を超えるダメージ）
    bob.takeDamage(150);

    // 死亡状態でのアクション確認
    bob.attack("Alice");
    bob.beRepaired(10);
    bob.highFivesGuys();

    std::cout << "\n--- デストラクタのテスト (連鎖の確認) ---" << std::endl;
    return 0;
}
