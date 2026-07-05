# include "ClapTrap.hpp"

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

ClapTrap::ClapTrap()
	: _name("Default"), _hitPoints(10), _energyPoints(10), _attackDamage(0) {
		std::cout << "ClapTrap Default constructor called for " << this->_name << std::endl;
	}

ClapTrap::ClapTrap(const std::string& name)
	: _name(name), _hitPoints(10), _energyPoints(10), _attackDamage(0) {
		std::cout << "ClapTrap Constructor called for " << this->_name << std::endl;
	}

ClapTrap::ClapTrap(const ClapTrap& src)
    : _name(src._name),
      _hitPoints(src._hitPoints),
      _energyPoints(src._energyPoints),
      _attackDamage(src._attackDamage)
{
    std::cout << "ClapTrap Copy constructor called" << std::endl;
}

ClapTrap& ClapTrap::operator=(const ClapTrap& rhs) {
	std::cout << "ClapTrap Copy assignment operator called" << std::endl;
	if (this != &rhs) {
		this->_name = rhs._name;
        this->_hitPoints = rhs._hitPoints;
        this->_energyPoints = rhs._energyPoints;
        this->_attackDamage = rhs._attackDamage;
	}
	return *this;
}

ClapTrap::~ClapTrap() {
	std::cout << "ClapTrap Destructor called for " << this->_name << std::endl;
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

void ClapTrap::attack(const std::string& target) {
	if (this->_hitPoints == 0 || this->_energyPoints == 0) {
		std::cout << "ClapTrap " << this->_name
                  << " can't attack (No hit points or energy points left)!" << std::endl;
        return;
	}

	this->_energyPoints--;
	std::cout << "ClapTrap " << this->_name << " attacks " << target
              << ", causing " << this->_attackDamage << " points of damage!" << std::endl;
}

void ClapTrap::takeDamage(unsigned int amount) {
	if (this->_hitPoints == 0) {
        std::cout << "ClapTrap " << this->_name << " is already destroyed!" << std::endl;
        return;
    }

	if (amount >= this->_hitPoints) {
		this->_hitPoints = 0;
	} else {
        this->_hitPoints -= amount;
    }

	std::cout << "ClapTrap " << this->_name << " takes " << amount
              << " points of damage! (HP left: " << this->_hitPoints << ")" << std::endl;
}

void ClapTrap::beRepaired(unsigned int amount) {
	if (this->_hitPoints == 0 || this->_energyPoints == 0) {
		std::cout << "ClapTrap " << this->_name
                  << " can't be repaired (No hit points or energy points left)!" << std::endl;
        return;
	}

	this->_energyPoints--;
    this->_hitPoints += amount;
	std::cout << "ClapTrap " << this->_name << " repairs itself, recovering "
              << amount << " hit points! (HP is now: " << this->_hitPoints << ")" << std::endl;
}
