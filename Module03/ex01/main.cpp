#include "ScavTrap.hpp"
#include <iostream>

int main() {
    std::cout << "--- コンストラクタとOCFのテスト ---" << std::endl;
    // 名前付きコンストラクタ
    ScavTrap alpha("Alpha");
    ScavTrap beta("Beta");
	std::cout  << std::endl;

    // デフォルトコンストラクタと代入演算子
    ScavTrap gamma;
    gamma = alpha;
	std::cout << std::endl;

    // コピーコンストラクタ
    ScavTrap delta(beta);

    std::cout << "\n--- 基本アクションとオーバーライドのテスト ---" << std::endl;
    // 親クラスの attack ではなく、ScavTrap の attack が呼ばれるか確認
    // 攻撃力が 20 に設定されているかもメッセージから確認できる
    alpha.attack("Enemy");

    // 親クラス (ClapTrap) から引き継いだ takeDamage と beRepaired の動作確認
    // HPが 100 に拡張されているため、50 のダメージでも破壊されない
    alpha.takeDamage(50);
    alpha.beRepaired(30);

    std::cout << "\n--- 特殊能力 (guardGate) のテスト ---" << std::endl;
    alpha.guardGate();

    std::cout << "\n--- 死亡時の動作テスト ---" << std::endl;
    // Beta に致命傷 (HP 100 を超えるダメージ) を与える
    beta.takeDamage(150);

    // 死亡状態でアクションを起こそうとしたときの挙動確認
    beta.attack("Enemy");
    beta.beRepaired(10);
    beta.guardGate();

    std::cout << "\n--- デストラクタのテスト (連鎖の確認) ---" << std::endl;

	// ここで return 0; に達すると、ローカル変数がスタックから破棄
    return 0;
}
