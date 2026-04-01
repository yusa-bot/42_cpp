// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

#include "Zombie.hpp"
#include <iostream>

// デストラクタ
// デバッグ目的でゾンビの名前を含むメッセージを出力する必要
Zombie::~Zombie(void) {
    std::cout << this->name << " is destroyed." << std::endl;
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

//ゾンビは次のように自己紹介します： <name>: BraiiiiiiinnnzzzZ...
void Zombie::announce( void ) {
    std::cout << this->name << ": BraiiiiiiinnnzzzZ..." << std::endl;
}

void Zombie::setName(std::string name) {
    this->name = name;
}
