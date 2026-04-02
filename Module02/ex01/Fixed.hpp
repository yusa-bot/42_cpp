#ifndef FIXED_HPP
# define FIXED_HPP

# include <iostream>

class Fixed {

private:
	int					_fixedPointValue;
	static const int    _fractionalBits = 8;

public:
	// OCF
	Fixed();
	Fixed(const Fixed& src);
	Fixed& operator=(const Fixed& rhs);
    ~Fixed();

	// コンストラクタ
	// 定数整数 -> 固定小数点値
	Fixed(int const n);
	// 定数浮動小数点数 -> 固定小数点値
	Fixed(float const f);

	// getter, setter
	int getRawBits(void) const;
	void setRawBits(int const raw);

	// 変換
	// 固定小数点値を浮動小数点値に変換
	float toFloat( void ) const;
	// 固定小数点値を整数値に変換
	int toInt( void ) const;
};

// 挿入演算子 («) のオーバーロード
// 固定小数点数の浮動小数点表現を、パラメータとして渡された出力ストリームオブジェクトに挿入
std::ostream& operator<<(std::ostream& out, const Fixed& fixed);

#endif
