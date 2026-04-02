#include <iostream>
#include "Fixed.hpp"

int main( void ) {
	Fixed a;

	// <<8で整数に -> roundf -> *ではtoFloat() -> <<でtoFloat() -> std::cout有効数字6桁のため四捨五入
	Fixed const b( Fixed( 5.05f ) * Fixed( 2 ) );

	std::cout << a << std::endl;
	std::cout << ++a << std::endl;
	std::cout << a << std::endl;
	std::cout << a++ << std::endl;
	std::cout << a << std::endl;
	std::cout << b << std::endl;
	std::cout << Fixed::max( a, b ) << std::endl;
	return 0;
}

// 1/256: 0.00390625
