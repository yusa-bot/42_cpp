#include <iostream>
#include "Fixed.hpp"

int main( void ) {

	// Default constructor called
	Fixed a;

	// Int constructor called
	Fixed const b( 10 );

	// Float constructor called
	Fixed const c( 42.42f );

	//Copy constructor called
	//Copy assignment operator called
	//getRawBits member function called
	Fixed const d( b );

	//Float constructor called
	//Copy assignment operator called
	//getRawBits member function called
	// Destructor called
	a = Fixed( 1234.4321f );

	std::cout << "a is " << a << std::endl;
	std::cout << "b is " << b << std::endl;
	//  toFloat -> std::cout（有効数字6桁）
	std::cout << "c is " << c << std::endl;
	std::cout << "d is " << d << std::endl;

	std::cout << "a is " << a.toInt() << " as integer" << std::endl;
	std::cout << "b is " << b.toInt() << " as integer" << std::endl;
	std::cout << "c is " << c.toInt() << " as integer" << std::endl;
	std::cout << "d is " << d.toInt() << " as integer" << std::endl;
	return 0;
}
