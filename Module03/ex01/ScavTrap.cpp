#include "ScavTrap.hpp"
#include <iostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

// ClapTrapが先に走る　-> ScavTrapで値を変える必要がある変数の値を上書き
ScavTrap::ScavTrap() : ClapTrap() {
	this->_hitPoints = 100;
    this->_energyPoints = 50;
    this->_attackDamage = 20;
	std::cout << "ScavTrap Default constructor called for " << this->_name << std::endl;
}

// ClapTrapが先に走る　-> ScavTrapで値を変える必要がある変数の値を上書き
ScavTrap::ScavTrap(const std::string& name) : ClapTrap(name) {
    this->_hitPoints = 100;
    this->_energyPoints = 50;
    this->_attackDamage = 20;
    std::cout << "ScavTrap Constructor called for " << this->_name << std::endl;
}

ScavTrap::ScavTrap(const ScavTrap& src) : ClapTrap(src) {
    std::cout << "ScavTrap Copy constructor called" << std::endl;
}

// Copy constructorはClapTrapのものを使うためCopy assignment operatorは呼ばないが、代入操作で使用
ScavTrap& ScavTrap::operator=(const ScavTrap& rhs) {
	std::cout << "ScavTrap Copy assignment operator called" << std::endl;
    if (this != &rhs) {
        // 親クラスの代入演算子を呼び出して、継承元の変数を正しくコピー
        ClapTrap::operator=(rhs);
    }
    return *this;
}

ScavTrap::~ScavTrap() {
    std::cout << "ScavTrap Destructor called for " << this->_name << std::endl;
}

// -----------------------------------------------------------------------------
// メンバ関数のオーバーライド
// -----------------------------------------------------------------------------

void ScavTrap::attack(const std::string& target) {
	if (this->_hitPoints == 0 || this->_energyPoints == 0) {
		std::cout << "ScavTrap " << this->_name
                  << " can't attack (No hit points or energy points left)!" << std::endl;
        return;
	}

	this->_energyPoints--;
    std::cout << "ScavTrap " << this->_name << " violently attacks " << target
              << ", causing " << this->_attackDamage << " points of damage!" << std::endl;
}

void ScavTrap::guardGate() {
	if (this->_hitPoints == 0) {
        std::cout << "ScavTrap " << this->_name
                  << " is dead and cannot enter Gate keeper mode." << std::endl;
        return;
    }
    std::cout << "ScavTrap " << this->_name << " is now in Gate keeper mode." << std::endl;
}
