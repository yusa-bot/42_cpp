#include "Fixed.hpp"
#include <iostream>
#include <cmath>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

Fixed::Fixed() : _fixedPointValue(0) {
	std::cout << "Default constructor called" << std::endl;
}

Fixed::Fixed(const Fixed& src) {
	std::cout << "Copy constructor called" << std::endl;
	*this = src;
}

Fixed& Fixed::operator=(const Fixed& rhs) {
	std::cout << "Copy assignment operator called" << std::endl;
	if (this != &rhs) {
		this->_fixedPointValue = rhs.getRawBits();
	}
	return *this;
}

Fixed::~Fixed() {
	std::cout << "Destructor called" << std::endl;
}

// -----------------------------------------------------------------------------
// 追加コンストラクタ
// -----------------------------------------------------------------------------

// 定数 整数 -> 固定小数点値
Fixed::Fixed(const int n) {
	std::cout << "Int constructor called" << std::endl;

	// 小数部ビット数を格納する桁数分シフト(整数n x 256)
	// 固定小数点数では、256で1 (_fractionalBitsが8だから)
	this->_fixedPointValue = n << _fractionalBits;
}

// 定数 浮動小数点数 -> 固定小数点値
Fixed::Fixed(const float f) {
	std::cout << "Float constructor called" << std::endl;

	// 浮動小数点数を2^8(256)倍 (floatにはシフトできないため、掛け算で代用)
	this->_fixedPointValue = roundf(f * (1 << _fractionalBits));
}

// -> 固定小数点は_fractionalBitsに基づいて操作する

// -----------------------------------------------------------------------------
// メンバ関数 (getter, setter)
// -----------------------------------------------------------------------------

int Fixed::getRawBits(void) const {
	std::cout << "getRawBits member function called" << std::endl;
	return this->_fixedPointValue;
}

void Fixed::setRawBits(int const raw) {
	this->_fixedPointValue = raw;
}

// -----------------------------------------------------------------------------
// メンバ関数 (変換)
// -----------------------------------------------------------------------------

// 固定小数点値 -> 浮動小数点値
float Fixed::toFloat(void) const {
	// 固定小数点数を 2^8 (256.0f) で割る (シフト演算の代用)
	return (float)this->_fixedPointValue / (float)(1 << _fractionalBits);
}

// 固定小数点値 -> 整数値
int Fixed::toInt(void) const {
	// 固定小数点数を右に8ビットシフト
	return this->_fixedPointValue >> _fractionalBits;
}

// -----------------------------------------------------------------------------
// 非メンバ関数
// -----------------------------------------------------------------------------


std::ostream& operator<<(std::ostream& out, const Fixed& fixed) {
	// toFloat() を呼び出して、浮動小数点数として出力ストリームに挿入
	out << fixed.toFloat();
	return out;
}
