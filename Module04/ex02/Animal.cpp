#include "Animal.hpp"
#include <iostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

Animal::Animal() : type("") {
    std::cout << "Animal default constructor called" << std::endl;
}

Animal::Animal(const Animal& src) {
    std::cout << "Animal copy constructor called" << std::endl;
    *this = src;
}

Animal& Animal::operator=(const Animal& rhs) {
    std::cout << "Animal copy assignment operator called" << std::endl;
    if (this != &rhs) {
        this->type = rhs.type;
    }
    return *this;
}

Animal::~Animal() {
    std::cout << "Animal destructor called" << std::endl;
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

std::string Animal::getType() const {
    return this->type;
}
