# include "FragTrap.hpp"

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

FragTrap::FragTrap() : ClapTrap() {
	this->_hitPoints = 100;
    this->_energyPoints = 100;
    this->_attackDamage = 30;
	std::cout << "FragTrap Default constructor called for " << this->_name << std::endl;
}

FragTrap::FragTrap(const std::string& name) : ClapTrap(name) {
    this->_hitPoints = 100;
    this->_energyPoints = 100;
    this->_attackDamage = 30;
    std::cout << "FragTrap Constructor called for " << this->_name << std::endl;
}

FragTrap::FragTrap(const FragTrap& src) : ClapTrap(src) {
    std::cout << "FragTrap Copy constructor called" << std::endl;
}

FragTrap& FragTrap::operator=(const FragTrap& rhs) {
	std::cout << "FragTrap Copy assignment operator called" << std::endl;
    if (this != &rhs) {
        ClapTrap::operator=(rhs);
    }
    return *this;
}

FragTrap::~FragTrap() {
    std::cout << "FragTrap Destructor called for " << this->_name << std::endl;
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

void FragTrap::highFivesGuys(void) {
	if (this->_hitPoints == 0) {
        std::cout << "FragTrap " << this->_name << " is dead and cannot high five." << std::endl;
        return;
    }
    std::cout << "FragTrap " << this->_name << " says: \"Hey guys! Give me a high five! ✋\"" << std::endl;
}
