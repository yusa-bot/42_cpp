#include "Fixed.hpp"
#include <iostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

Fixed::Fixed() : _fixedPointValue(0) {
	std::cout << "Default constructor called" << std::endl;
}

Fixed::Fixed(const Fixed& src) { // 参照渡し
	std::cout << "Copy constructor called" << std::endl;
	// this:現在のFixedのポインタ, *this:実体(デリファレンス)
	*this = src; // ここで代入演算子を呼ぶ
}

// 参照を返す
Fixed& Fixed::operator=(const Fixed& rhs) {
	std::cout << "Copy assignment operator called" << std::endl;

	if (this != &rhs) { // this:*Fixed型, rhs:Fixed型なので&rhsにして比較
		this->_fixedPointValue = rhs.getRawBits();
	}
	return *this; // 実体
}

Fixed::~Fixed() {
	std::cout << "Destructor called" << std::endl;
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

int Fixed::getRawBits(void) const {
	std::cout << "getRawBits member function called" << std::endl;
	return this->_fixedPointValue;
}

void Fixed::setRawBits(int const raw) {
	this->_fixedPointValue = raw;
}
