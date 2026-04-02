#include "Fixed.hpp"
#include <iostream>
#include <cmath>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

Fixed::Fixed() : _fixedPointValue(0) {}

// 定数 整数 -> 固定小数点値
Fixed::Fixed(const int n) {
	this->_fixedPointValue = n << _fractionalBits;
}

// 定数 浮動小数点数 -> 固定小数点値
Fixed::Fixed(const float f) {
	this->_fixedPointValue = roundf(f * (1 << _fractionalBits));
}

Fixed::Fixed(const Fixed& src) {
	*this = src;
}

Fixed& Fixed::operator=(const Fixed& rhs) {
	if (this != &rhs) {
		this->_fixedPointValue = rhs.getRawBits();
	}
	return *this;
}

Fixed::~Fixed() {}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

int Fixed::getRawBits(void) const {
	return this->_fixedPointValue;
}

void Fixed::setRawBits(int const raw) {
	this->_fixedPointValue = raw;
}

// 固定小数点値 -> 浮動小数点値
float Fixed::toFloat(void) const {
	return (float)this->_fixedPointValue / (float)(1 << _fractionalBits);
}

// 固定小数点値 -> 整数値
int Fixed::toInt(void) const {
	return this->_fixedPointValue >> _fractionalBits;
}

// 比較演算子
// -----------------------------------------------------------------------------

bool Fixed::operator>(const Fixed& rhs) const {
	return this->getRawBits() > rhs.getRawBits();
}

bool Fixed::operator<(const Fixed& rhs) const {
    return this->getRawBits() < rhs.getRawBits();
}

bool Fixed::operator>=(const Fixed& rhs) const {
    return this->getRawBits() >= rhs.getRawBits();
}

bool Fixed::operator<=(const Fixed& rhs) const {
    return this->getRawBits() <= rhs.getRawBits();
}

bool Fixed::operator==(const Fixed& rhs) const {
    return this->getRawBits() == rhs.getRawBits();
}

bool Fixed::operator!=(const Fixed& rhs) const {
    return this->getRawBits() != rhs.getRawBits();
}

// 算術演算子
// -----------------------------------------------------------------------------

// 小数点の位置は8bitで固定のため生のbitを計算に使える

Fixed Fixed::operator+(const Fixed& rhs) const {
	Fixed result;
	result.setRawBits(this->getRawBits() + rhs.getRawBits());
	return result;
}

Fixed Fixed::operator-(const Fixed& rhs) const {
    Fixed result;
    result.setRawBits(this->getRawBits() - rhs.getRawBits());
    return result;
}

Fixed Fixed::operator*(const Fixed& rhs) const {
    // 乗算はビットシフトで計算するとオーバーフローのリスクがあるため、
    // 一度 float に変換して掛け合わせ、その結果から新しい Fixed オブジェクトを作る
    return Fixed(this->toFloat() * rhs.toFloat());
}

Fixed Fixed::operator/(const Fixed& rhs) const {
    // 除算も乗算と同様に、float に変換して計算
    return Fixed(this->toFloat() / rhs.toFloat());
}

// 増減演算子
// -----------------------------------------------------------------------------

// ϵ: 生のビット値における1

// ++a
Fixed& Fixed::operator++(void) {
	this->_fixedPointValue++;
	return *this; // 実体
}

// a++
Fixed Fixed::operator++(int) {
	Fixed tmp(*this); // 増やすをコピー
	this->_fixedPointValue++;
	return tmp;
}

// -a
Fixed& Fixed::operator--(void) {
    this->_fixedPointValue--;
    return *this;
}

// a--
Fixed Fixed::operator--(int) {
    Fixed tmp(*this);
    this->_fixedPointValue--;
    return tmp;
}

// 静的メンバ関数
// -----------------------------------------------------------------------------

Fixed& Fixed::min(Fixed& a, Fixed& b) {
	return (a < b) ? a : b;
}

const Fixed& Fixed::min(const Fixed& a, const Fixed& b) {
    return (a < b) ? a : b;
}

Fixed& Fixed::max(Fixed& a, Fixed& b) {
    return (a > b) ? a : b;
}

const Fixed& Fixed::max(const Fixed& a, const Fixed& b) {
    return (a > b) ? a : b;
}

// -----------------------------------------------------------------------------
// 非メンバ関数
// -----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& out, const Fixed& fixed) {
	// toFloat() を呼び出して、浮動小数点数として出力ストリームに挿入
	out << fixed.toFloat();
	return out;
}
