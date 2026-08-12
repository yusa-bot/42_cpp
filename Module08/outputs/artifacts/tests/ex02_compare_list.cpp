#include "MutantStack.hpp"

#include <iostream>
#include <list>
#include <sstream>

template <typename Iterator>
static std::string serialize(Iterator first, Iterator last) {
    std::ostringstream output;
    for (; first != last; ++first)
        output << *first << ' ';
    return output.str();
}

int main() {
    MutantStack<int> stack;
    std::list<int> list;

    stack.push(5);
    list.push_back(5);
    stack.push(17);
    list.push_back(17);
    stack.pop();
    list.pop_back();
    stack.push(3);
    list.push_back(3);
    stack.push(5);
    list.push_back(5);
    stack.push(737);
    list.push_back(737);
    stack.push(0);
    list.push_back(0);

    const std::string stackValues = serialize(stack.begin(), stack.end());
    const std::string listValues = serialize(list.begin(), list.end());
    std::cout << "MutantStack: " << stackValues << std::endl;
    std::cout << "std::list:   " << listValues << std::endl;
    std::cout << "equal: " << (stackValues == listValues) << std::endl;
    return stackValues == listValues ? 0 : 1;
}
