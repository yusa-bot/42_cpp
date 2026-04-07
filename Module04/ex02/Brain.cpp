#include "Brain.hpp"
#include <iostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

Brain::Brain() {
	std::cout << "Brain default constructor called" << std::endl;
}

Brain::Brain(const Brain& src) {
	std::cout << "Brain copy constructor called" << std::endl;
	*this = src;
}

Brain& Brain::operator=(const Brain& rhs) {
	std::cout << "Brain copy assignment operator called" << std::endl;
	if (this != &rhs) {
        for (int i = 0; i < 100; i++) {
            this->ideas[i] = rhs.ideas[i];
        }
    }
	return *this;
}

Brain::~Brain() {
    std::cout << "Brain destructor called" << std::endl;
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

void Brain::setIdea(int index, const std::string& idea) {
	if (index >= 0 && index < 100) {
        this->ideas[index] = idea;
    }
}

std::string Brain::getIdea(int index) const {
	if (index >= 0 && index < 100) {
        return this->ideas[index];
    }
    return "";
}
