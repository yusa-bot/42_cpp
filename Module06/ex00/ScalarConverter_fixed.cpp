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

enum LiteralType {
    TYPE_CHAR,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_INVALID
};

struct ParsedLiteral {
    LiteralType type;
    char charValue;
    int intValue;
    float floatValue;
    double doubleValue;

    ParsedLiteral()
        : type(TYPE_INVALID), charValue(0), intValue(0), floatValue(0.0f),
          doubleValue(0.0) {}
};





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

static bool isPseudoLiteral(const std::string& literal, ParsedLiteral& parsed) {
    if (literal == "nanf" || literal == "+inff" || literal == "-inff") {
        parsed.type = TYPE_FLOAT;
        if (literal == "nanf")
            parsed.floatValue = std::numeric_limits<float>::quiet_NaN();
        else if (literal == "-inff")
            parsed.floatValue = -std::numeric_limits<float>::infinity();
        else
            parsed.floatValue = std::numeric_limits<float>::infinity();
        return true;
    }
    if (literal == "nan" || literal == "+inf" || literal == "-inf") {
        parsed.type = TYPE_DOUBLE;
        if (literal == "nan")
            parsed.doubleValue = std::numeric_limits<double>::quiet_NaN();
        else if (literal == "-inf")
            parsed.doubleValue = -std::numeric_limits<double>::infinity();
        else
            parsed.doubleValue = std::numeric_limits<double>::infinity();
        return true;
    }
    return false;
}

static bool isCharLiteral(const std::string& literal, ParsedLiteral& parsed) {
    if (literal.length() == 3 && literal[0] == '\'' && literal[2] == '\'') {
        parsed.type = TYPE_CHAR;
        parsed.charValue = literal[1];
        return true;
    }
    if (literal.length() == 1
        && !std::isdigit(static_cast<unsigned char>(literal[0]))) {
        parsed.type = TYPE_CHAR;
        parsed.charValue = literal[0];
        return true;
    }
    return false;
}





static bool hasDecimalSyntax(const std::string& number, bool& isFloatingPoint) {
    std::string::size_type index = 0;
    bool hasDigitsBeforePoint = false;
    bool hasDigitsAfterPoint = false;
    bool hasPoint = false;
    bool hasExponent = false;

    if (index < number.length()
        && (number[index] == '+' || number[index] == '-'))
        ++index;
    while (index < number.length()
           && std::isdigit(static_cast<unsigned char>(number[index]))) {
        hasDigitsBeforePoint = true;
        ++index;
    }
    if (index < number.length() && number[index] == '.') {
        hasPoint = true;
        ++index;
        while (index < number.length()
               && std::isdigit(static_cast<unsigned char>(number[index]))) {
            hasDigitsAfterPoint = true;
            ++index;
        }
    }
    if (!hasDigitsBeforePoint && !hasDigitsAfterPoint)
        return false;

    if (index < number.length()
        && (number[index] == 'e' || number[index] == 'E')) {
        bool hasExponentDigits = false;

        hasExponent = true;
        ++index;
        if (index < number.length()
            && (number[index] == '+' || number[index] == '-'))
            ++index;
        while (index < number.length()
               && std::isdigit(static_cast<unsigned char>(number[index]))) {
            hasExponentDigits = true;
            ++index;
        }
        if (!hasExponentDigits)
            return false;
    }
    isFloatingPoint = hasPoint || hasExponent;
    return index == number.length();
}

static bool parseInt(const std::string& number, ParsedLiteral& parsed) {
    char* end;
    long value;

    errno = 0;
    value = std::strtol(number.c_str(), &end, 10);
    if (*end != '\0' || errno == ERANGE || value < INT_MIN || value > INT_MAX)
        return false;
    parsed.type = TYPE_INT;
    parsed.intValue = static_cast<int>(value);
    return true;
}

static bool parseFloat(const std::string& number, ParsedLiteral& parsed) {
    char* end;
    float value;

    errno = 0;
    value = std::strtof(number.c_str(), &end);
    if (*end != '\0' || isInfinite(static_cast<double>(value)))
        return false;
    parsed.type = TYPE_FLOAT;
    parsed.floatValue = value;
    return true;
}

static bool parseDouble(const std::string& number, ParsedLiteral& parsed) {
    char* end;
    double value;

    errno = 0;
    value = std::strtod(number.c_str(), &end);
    if (*end != '\0' || isInfinite(value))
        return false;
    parsed.type = TYPE_DOUBLE;
    parsed.doubleValue = value;
    return true;
}

static bool parseNumber(const std::string& literal, ParsedLiteral& parsed) {
    std::string number = literal;
    bool hasFloatSuffix = false;
    bool isFloatingPoint = false;

    if (number.length() > 1 && number[number.length() - 1] == 'f') {
        hasFloatSuffix = true;
        number.erase(number.length() - 1);
    }
    if (!hasDecimalSyntax(number, isFloatingPoint))
        return false;
    if (hasFloatSuffix) {
        if (!isFloatingPoint)
            return false;
        return parseFloat(number, parsed);
    }
    if (isFloatingPoint)
        return parseDouble(number, parsed);
    return parseInt(number, parsed);
}





static std::string formatFloat(float value) {
    std::ostringstream stream;
    std::string formatted;

    stream << std::setprecision(7) << value;
    formatted = stream.str();
    if (value == std::floor(value) // 小数部分があるか
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





static void printCharImpossible() {
    std::cout << "char: impossible" << std::endl;
}

static void printChar(char value) {
    if (!std::isprint(static_cast<unsigned char>(value)))
        std::cout << "char: Non displayable" << std::endl;
    else
        std::cout << "char: '" << value << "'" << std::endl;
}

static void printIntImpossible() {
    std::cout << "int: impossible" << std::endl;
}

static void printInt(int value) {
    std::cout << "int: " << value << std::endl;
}

static void printFloatImpossible() {
    std::cout << "float: impossible" << std::endl;
}

static void printFloat(float value) {
    if (isNaN(static_cast<double>(value)))
        std::cout << "float: nanf" << std::endl;
    else if (isInfinite(static_cast<double>(value)))
        std::cout << "float: " << (value < 0 ? "-inff" : "+inff")
                  << std::endl;
    else
        std::cout << "float: " << formatFloat(value) << "f" << std::endl;
}

static void printDouble(double value) {
    if (isNaN(value))
        std::cout << "double: nan" << std::endl;
    else if (isInfinite(value))
        std::cout << "double: " << (value < 0 ? "-inf" : "+inf")
                  << std::endl;
    else
        std::cout << "double: " << formatDouble(value) << std::endl;
}





static bool canConvertToChar(double value) {
    return !isSpecial(value) && value >= 0.0 && value <= 127.0;
}

static bool canConvertToInt(double value) {
    return !isSpecial(value)
        && value >= static_cast<double>(INT_MIN)
        && value <= static_cast<double>(INT_MAX);
}

static bool canConvertToFloat(double value) {
    return isSpecial(value)
        || (value >= -static_cast<double>(std::numeric_limits<float>::max())
            && value <= static_cast<double>(std::numeric_limits<float>::max()));
}





static void convertFromChar(char value) {
    printChar(value);
    printInt(static_cast<int>(value));
    printFloat(static_cast<float>(value));
    printDouble(static_cast<double>(value));
}

static void convertFromInt(int value) {
    if (value < 0 || value > 127)
        printCharImpossible();
    else
        printChar(static_cast<char>(value));
    printInt(value);
    printFloat(static_cast<float>(value));
    printDouble(static_cast<double>(value));
}

static void convertFromFloat(float value) {
    // charやintの範囲と安全に比較するため
    const double checkedValue = static_cast<double>(value);

    if (!canConvertToChar(checkedValue))
        printCharImpossible();
    else
        printChar(static_cast<char>(value));
    if (!canConvertToInt(checkedValue))
        printIntImpossible();
    else
        printInt(static_cast<int>(value));
    printFloat(value);
    printDouble(static_cast<double>(value));
}

static void convertFromDouble(double value) {
    if (!canConvertToChar(value))
        printCharImpossible();
    else
        printChar(static_cast<char>(value));
    if (!canConvertToInt(value))
        printIntImpossible();
    else
        printInt(static_cast<int>(value));
    if (!canConvertToFloat(value))
        printFloatImpossible();
    else
        printFloat(static_cast<float>(value));
    printDouble(value);
}





static void printImpossible() {
    printCharImpossible();
    printIntImpossible();
    printFloatImpossible();
    std::cout << "double: impossible" << std::endl;
}

void ScalarConverter::convert(const std::string& literal) {
    ParsedLiteral parsed;

    if (!isPseudoLiteral(literal, parsed)
        && !isCharLiteral(literal, parsed)
        && !parseNumber(literal, parsed)) {
        printImpossible();
        return;
    }
    if (parsed.type == TYPE_CHAR)
        convertFromChar(parsed.charValue);
    else if (parsed.type == TYPE_INT)
        convertFromInt(parsed.intValue);
    else if (parsed.type == TYPE_FLOAT)
        convertFromFloat(parsed.floatValue);
    else if (parsed.type == TYPE_DOUBLE)
        convertFromDouble(parsed.doubleValue);
    else
        printImpossible();
}
