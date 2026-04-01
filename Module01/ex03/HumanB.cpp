#include "HumanB.hpp"
#include <iostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

HumanB::HumanB(std::string name) : _name(name), _weapon(NULL) {}

HumanB::~HumanB() {}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

void HumanB::attack(void) const {
    if (this->_weapon) {
        std::cout << this->_name << " attacks with their "
                  << this->_weapon->getType() << std::endl;
    } else {
        std::cout << this->_name << " has no weapon to attack with!" << std::endl;
    }
}

// 引数Weapon&: 値の別名 -> 代入時: _weaponは*のため、値(weapon)の&を代入
void HumanB::setWeapon(Weapon& weapon) {
    this->_weapon = &weapon;
}
