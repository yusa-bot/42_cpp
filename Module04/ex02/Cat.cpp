#include "Cat.hpp"
#include <iostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

Cat::Cat() : Animal() {
    std::cout << "Cat default constructor called" << std::endl;
	this->type = "Cat";
    this->_brain = new Brain();
}

Cat::Cat(const Cat& src) : Animal(src) {
    std::cout << "Cat copy constructor called" << std::endl;

    // _brain以外は親でcopy

    this->_brain = new Brain(*(src._brain)); // _brainはBrain*なのでデリファレンス
}

Cat& Cat::operator=(const Cat& rhs) {
    std::cout << "Cat copy assignment operator called" << std::endl;

    if (this != &rhs) {

        Animal::operator=(rhs); // _brain以外は親でcopy

        // 古い Brain を破棄し、新しい Brain を確保してコピーする
        Brain* tmp = new Brain(*(rhs._brain));
        // new失敗時、std::bad_allocという例外をスロー
        // -> delete を実行する前に例外で関数を抜けるため、元のメモリは解放されず、ポインタも有効 (元の_brainのまま)
        delete this->_brain;
        this->_brain = tmp;
    }
    return *this;
}

Cat::~Cat() {
    std::cout << "Cat destructor called" << std::endl;
    delete this->_brain;
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

void Cat::makeSound() const {
	std::cout << "にゃー" << std::endl;
}

void Cat::setIdea(int index, const std::string& idea) {
    this->_brain->setIdea(index, idea); // 親のsetIdea
}

std::string Cat::getIdea(int index) const {
    return this->_brain->getIdea(index); // 親のgetIdea
}
