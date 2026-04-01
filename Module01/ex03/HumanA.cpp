#include "HumanA.hpp"
#include <iostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

// 参照のため、初期化の時に値を代入
HumanA::HumanA(std::string name, Weapon& weapon) : _name(name), _weapon(weapon) {}

HumanA::~HumanA() {}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

void HumanA::attack(void) const {
    std::cout << this->_name << " attacks with their "
              << this->_weapon.getType() << std::endl;
}
