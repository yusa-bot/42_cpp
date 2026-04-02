#include "Dog.hpp"
#include <iostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

Dog::Dog() : Animal() {
    std::cout << "Dog default constructor called" << std::endl;
	this->type = "Dog";
}

Dog::Dog(const Dog& src) : Animal(src) {
    std::cout << "Dog copy constructor called" << std::endl;
}

Dog& Dog::operator=(const Dog& rhs) {
    std::cout << "Dog copy assignment operator called" << std::endl;
    if (this != &rhs) {
        this->type = rhs.type;
    }
    return *this;
}

Dog::~Dog() {
    std::cout << "Dog destructor called" << std::endl;
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

void Dog::makeSound() const {
	std::cout << "わん" << std::endl;
}
