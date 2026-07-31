#ifndef SCALAR_CONVERTER_HPP
# define SCALAR_CONVERTER_HPP

# include <string>

// ex目的: 基本型同士の変換(static_cast), static関数

// ScalarConverterは機能のみの提供で、変数を持つ必要がないため、static
// staticはインスタンス化せず、クラス名から呼べる. thisを持たない

class ScalarConverter {
// main.cppからはインスタンス化できない
private:
    ScalarConverter();
    ScalarConverter(const ScalarConverter& src);
    ScalarConverter& operator=(const ScalarConverter& rhs);
    ~ScalarConverter();

public:
    static void convert(const std::string& literal);
};

#endif

// 変換条件
// input: charは1文字, numはdicのみ, non-dis不要, f, pseudo-literals
//      pseudo-literals: nan, +inf, -inf, nanf, +inff, -inff
// output: non-dis
