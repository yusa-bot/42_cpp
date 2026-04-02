#include "WrongCat.hpp"
#include <iostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

WrongCat::WrongCat() : WrongAnimal() {
    std::cout << "WrongCat default constructor called" << std::endl;
	this->type = "WrongCat";
}

WrongCat::WrongCat(const WrongCat& src) : WrongAnimal(src) {
    std::cout << "WrongCat copy constructor called" << std::endl;
    *this = src;
}

WrongCat& WrongCat::operator=(const WrongCat& rhs) {
    std::cout << "WrongCat copy assignment operator called" << std::endl;
    if (this != &rhs) {
        this->type = rhs.type;
    }
    return *this;
}

WrongCat::~WrongCat() {
    std::cout << "WrongCat destructor called" << std::endl;
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

void WrongCat::makeSound() const {
	std::cout << "にゃー" << std::endl;
}
