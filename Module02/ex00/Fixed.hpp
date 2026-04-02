#ifndef FIXED_HPP
# define FIXED_HPP

class Fixed {

private:
	// 固定小数点数の値を格納するための整数型
	int					_fixedPointValue;
	// 小数部ビット数を格納するための静的定数整数。その値は常に整数リテラル 8
	static const int    _fractionalBits = 8;

public:
	// OCF: Fixedでもintなどと同じように、数値を扱えるようにするため
	// デフォルトコンストラクタ
	Fixed();
	// コピーコンストラクタ
	Fixed(const Fixed& src);
	// コピー代入演算子
	Fixed& operator=(const Fixed& rhs);
	// デストラクタ
    ~Fixed();

	// 固定小数点値の生の値を返すメンバ関数
	int getRawBits(void) const;
	//  固定小数点数の生の値を設定するメンバ関数
	void setRawBits(int const raw);
};

#endif
