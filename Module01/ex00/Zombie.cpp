#include "Zombie.hpp"
#include <iostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

// コンストラクタ
Zombie::Zombie(std::string name) : name(name) {}

// デストラクタ
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
