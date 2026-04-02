#include "WrongAnimal.hpp"
#include <iostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

WrongAnimal::WrongAnimal() : type("") {
    std::cout << "WrongAnimal default constructor called" << std::endl;
}

WrongAnimal::WrongAnimal(const WrongAnimal& src) {
    std::cout << "WrongAnimal copy constructor called" << std::endl;
    *this = src;
}

WrongAnimal& WrongAnimal::operator=(const WrongAnimal& rhs) {
    std::cout << "WrongAnimal copy assignment operator called" << std::endl;
    if (this != &rhs) {
        this->type = rhs.type;
    }
    return *this;
}

WrongAnimal::~WrongAnimal() {
    std::cout << "WrongAnimal destructor called" << std::endl;
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

void WrongAnimal::makeSound() const {
    std::cout << "* Generic WrongAnimal sound *" << std::endl;
}

std::string WrongAnimal::getType() const {
    return this->type;
}
