#include "RPN.hpp"

#include <limits>
#include <sstream>
#include <stdexcept>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

RPN::RPN() : _values() {
}

RPN::RPN(const RPN& other) : _values(other._values) {
}

RPN& RPN::operator=(const RPN& other) {
    if (this != &other)
        _values = other._values;
    return *this;
}

RPN::~RPN() {
}

// -----------------------------------------------------------------------------
// private メンバ関数
// -----------------------------------------------------------------------------

bool RPN::isOperator(char token) {
    return token == '+' || token == '-' || token == '*' || token == '/';
}

void RPN::clear() {
    while (!_values.empty())
        _values.pop();
}

void RPN::applyOperator(char token) {
    if (_values.size() < 2)
        throw std::runtime_error("invalid expression");

    const int right = _values.top();
    _values.pop();
    const int left = _values.top();
    _values.pop();

    if (token == '/' && right == 0)
        throw std::runtime_error("division by zero");

    long result;
    switch (token) {
        case '+':
            result = static_cast<long>(left) + right;
            break;
        case '-':
            result = static_cast<long>(left) - right;
            break;
        case '*':
            result = static_cast<long>(left) * right;
            break;
        default:
            result = static_cast<long>(left) / right;
            break;
    }
    if (result < std::numeric_limits<int>::min()
        || result > std::numeric_limits<int>::max())
        throw std::runtime_error("integer overflow");
    _values.push(static_cast<int>(result));
}

// -----------------------------------------------------------------------------
// public メンバ関数
// -----------------------------------------------------------------------------

// expression ex.) "1 2 * 2 / 2 * 2 4 - +"
int RPN::evaluate(const std::string& expression) {
    clear();

    std::istringstream input(expression);
    std::string token;

    // 「空白まで」ごと
    while (input >> token) {
        if (token.size() == 1 && token[0] >= '0' && token[0] <= '9') {
            _values.push(token[0] - '0');
        } else if (token.size() == 1 && isOperator(token[0])) {
            applyOperator(token[0]);
        } else {
            throw std::runtime_error("invalid token");
        }
    }

    if (_values.size() != 1)
        throw std::runtime_error("invalid expression");

    return _values.top();
}
