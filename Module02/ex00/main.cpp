#include <iostream>
#include "Fixed.hpp"

int main( void ) {
	// デフォルト コンストラクタを呼ぶ
	Fixed a;
	// コピー コンストラクタを呼ぶ
	Fixed b( a );
	// デフォルト コンストラクタを呼ぶ
	Fixed c;

	// 代入演算子を呼ぶ
	c = b;

	std::cout << a.getRawBits() << std::endl;
	std::cout << b.getRawBits() << std::endl;
	std::cout << c.getRawBits() << std::endl;

	return 0;
}
