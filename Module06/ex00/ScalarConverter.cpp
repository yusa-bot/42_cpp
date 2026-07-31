#include "ScalarConverter.hpp"
#include <cerrno>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

static bool isPseudoLiteral(const std::string& literal, double& value) {
    if (literal == "nan" || literal == "nanf") {
        value = std::numeric_limits<double>::quiet_NaN();
        return true;
    }
    if (literal == "+inf" || literal == "+inff") {
        value = std::numeric_limits<double>::infinity();
        return true;
    }
    if (literal == "-inf" || literal == "-inff") {
        value = -std::numeric_limits<double>::infinity();
        return true;
    }
    return false;
}

static bool isCharLiteral(const std::string& literal, double& value) {
    if (literal.length() == 3 && literal[0] == '\'' && literal[2] == '\'') {
        value = static_cast<unsigned char>(literal[1]);
        return true;
    }
    if (literal.length() == 1 && !std::isdigit(static_cast<unsigned char>(literal[0]))) {
        value = static_cast<unsigned char>(literal[0]);
        return true;
    }
    return false;
}

static bool parseNumber(const std::string& literal, double& value) {
    std::string number = literal;
    char* end;

    if (number.length() > 1 && number[number.length() - 1] == 'f')
        number.erase(number.length() - 1);
    if (number.empty())
        return false;
    errno = 0;
    value = std::strtod(number.c_str(), &end);
    return *end == '\0' && errno != ERANGE;
}

static bool isNaN(double value) {
    return value != value;
}

static bool isInfinite(double value) {
    const double infinity = std::numeric_limits<double>::infinity();

    return value == infinity || value == -infinity;
}

static bool isSpecial(double value) {
    return isNaN(value) || isInfinite(value);
}

static std::string formatFloat(float value) {
    std::ostringstream stream;
    std::string formatted;

    stream << std::setprecision(7) << value;
    formatted = stream.str();
    if (value == std::floor(value)
        && formatted.find_first_of("eE") == std::string::npos)
        formatted += ".0";
    return formatted;
}

static std::string formatDouble(double value) {
    std::ostringstream stream;
    std::string formatted;

    stream << std::setprecision(15) << value;
    formatted = stream.str();
    if (value == std::floor(value)
        && formatted.find_first_of("eE") == std::string::npos)
        formatted += ".0";
    return formatted;
}

static void printChar(double value) {
    if (isSpecial(value) || value < 0 || value > 127)
        std::cout << "char: impossible" << std::endl;
    else if (!std::isprint(static_cast<unsigned char>(value)))
        std::cout << "char: Non displayable" << std::endl;
    else
        std::cout << "char: '" << static_cast<char>(value) << "'" << std::endl;
}

static void printInt(double value) {
    if (isSpecial(value) || value < INT_MIN || value > INT_MAX)
        std::cout << "int: impossible" << std::endl;
    else
        std::cout << "int: " << static_cast<int>(value) << std::endl;
}

static void printFloat(double value) {
    float converted;

    if (isNaN(value)) {
        std::cout << "float: nanf" << std::endl;
        return;
    }
    if (isInfinite(value)) {
        std::cout << "float: " << (value < 0 ? "-inff" : "+inff") << std::endl;
        return;
    }
    if (value < -std::numeric_limits<float>::max()
        || value > std::numeric_limits<float>::max()) {
        std::cout << "float: impossible" << std::endl;
        return;
    }
    converted = static_cast<float>(value);
    std::cout << "float: " << formatFloat(converted) << "f" << std::endl;
}

static void printDouble(double value) {
    if (isNaN(value)) {
        std::cout << "double: nan" << std::endl;
        return;
    }
    if (isInfinite(value)) {
        std::cout << "double: " << (value < 0 ? "-inf" : "+inf") << std::endl;
        return;
    }
    std::cout << "double: " << formatDouble(value) << std::endl;
}

void ScalarConverter::convert(const std::string& literal) {
    double value;

    // 入力文字列を値として解析できるか
    if (!isPseudoLiteral(literal, value)
        && !isCharLiteral(literal, value)
        && !parseNumber(literal, value)) {
        std::cout << "char: impossible" << std::endl;
        std::cout << "int: impossible" << std::endl;
        std::cout << "float: impossible" << std::endl;
        std::cout << "double: impossible" << std::endl;
        return;
    }

    // 解析済みの値を、その変換先の型で表現できるか
    printChar(value);
    printInt(value);
    printFloat(value);
    printDouble(value);
}
