#ifndef RPN_HPP
# define RPN_HPP

# include <stack>
# include <string>

class RPN {
private:
    std::stack<int> _values;

    static bool isOperator(char token);
    void applyOperator(char token);
    void clear();

public:
    RPN();
    RPN(const RPN& other);
    RPN& operator=(const RPN& other);
    ~RPN();

    int evaluate(const std::string& expression);
};

#endif
