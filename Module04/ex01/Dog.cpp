#include "Dog.hpp"
#include <iostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

Dog::Dog() : Animal() {
    std::cout << "Dog default constructor called" << std::endl;
	this->type = "Dog";
    this->_brain = new Brain();
}

Dog::Dog(const Dog& src) : Animal(src) {
    std::cout << "Dog copy constructor called" << std::endl;

    // _brain以外は親でcopy

    // _brainはBrain*なのでデリファレンス
    this->_brain = new Brain(*(src._brain));
}

Dog& Dog::operator=(const Dog& rhs) {
    std::cout << "Dog copy assignment operator called" << std::endl;

    if (this != &rhs) {

        // _brain以外は親でcopy
        Animal::operator=(rhs);

        // 古い Brain を破棄し、新しい Brain を確保してコピーする
        if (this->_brain) {
            delete this->_brain;
        }
        this->_brain = new Brain(*(rhs._brain));
    }
    return *this;
}

Dog::~Dog() {
    std::cout << "Dog destructor called" << std::endl;
    delete this->_brain;
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

void Dog::makeSound() const {
	std::cout << "わん" << std::endl;
}

void Dog::setIdea(int index, const std::string& idea) {
    this->_brain->setIdea(index, idea); // 親のsetIdea
}

std::string Dog::getIdea(int index) const {
    return this->_brain->getIdea(index); // 親のgetIdea
}
