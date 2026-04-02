#include "Cat.hpp"
#include <iostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

Cat::Cat() : Animal() {
    std::cout << "Cat default constructor called" << std::endl;
	this->type = "Cat";
}

Cat::Cat(const Cat& src) : Animal(src) {
    std::cout << "Cat copy constructor called" << std::endl;
}

Cat& Cat::operator=(const Cat& rhs) {
    std::cout << "Cat copy assignment operator called" << std::endl;
    if (this != &rhs) {
        this->type = rhs.type;
    }
    return *this;
}

Cat::~Cat() {
    std::cout << "Cat destructor called" << std::endl;
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

void Cat::makeSound() const {
	std::cout << "にゃー" << std::endl;
}
