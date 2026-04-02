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
	Fixed(int const n);
	Fixed(float const f);

	Fixed(const Fixed& src);
	Fixed& operator=(const Fixed& rhs);
    ~Fixed();

	int getRawBits(void) const;
	void setRawBits(int const raw);
	float toFloat( void ) const; // 固定小数点値を浮動小数点値
	int toInt( void ) const; // 固定小数点値を整数値

	// 追加 -----------------------------------------------------------------------------

	// 比較演算子
	bool operator>(const Fixed& rhs) const;
	bool operator<(const Fixed& rhs) const;
    bool operator>=(const Fixed& rhs) const;
    bool operator<=(const Fixed& rhs) const;
    bool operator==(const Fixed& rhs) const;
    bool operator!=(const Fixed& rhs) const;

	// 算術演算子
	Fixed operator+(const Fixed& rhs) const;
	Fixed operator-(const Fixed& rhs) const;
    Fixed operator*(const Fixed& rhs) const;
    Fixed operator/(const Fixed& rhs) const;

	// 増減演算子
	// これらは、1 + ϵ > 1 となるように固定小数点値を表現可能な最小の ϵ だけ増減させます
	Fixed&  operator++(void); // ++a
	Fixed   operator++(int); // a++
	Fixed&  operator--(void); // --a
    Fixed   operator--(int); // a--

	// min
	static Fixed& min(Fixed& a, Fixed& b);
	static const Fixed& min(const Fixed& a, const Fixed& b); // const
	// max
	static Fixed& max(Fixed& a, Fixed& b);
	static const Fixed& max(const Fixed& a, const Fixed& b); // const
};

// 挿入演算子 («) のオーバーロード
std::ostream& operator<<(std::ostream& out, const Fixed& fixed);

#endif
