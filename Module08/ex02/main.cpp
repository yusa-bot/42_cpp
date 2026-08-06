#include <iostream>
#include <list>
#include <stack>

#include "MutantStack.hpp"

int main() {
    MutantStack<int> mstack;
    mstack.push(5);
    mstack.push(17);
    std::cout << "top: " << mstack.top() << std::endl;
    mstack.pop();
    std::cout << "size: " << mstack.size() << std::endl;
    mstack.push(3);
    mstack.push(5);
    mstack.push(737);
    mstack.push(0);

    std::cout << "values:" << std::endl;
    for (MutantStack<int>::iterator it = mstack.begin();
         it != mstack.end(); ++it)
        std::cout << *it << std::endl;

    // const ver.
    const MutantStack<int> readOnly(mstack);
    std::cout << "const values:" << std::endl;
    for (MutantStack<int>::const_iterator it = readOnly.begin();
         it != readOnly.end(); ++it)
        std::cout << *it << std::endl;

    std::stack<int> standardStack(mstack);
    std::cout << "converted stack top: " << standardStack.top() << std::endl;

    // ----- list -----
    MutantStack<int, std::list<int> > listStack;
    listStack.push(10);
    listStack.push(20);
    std::cout << "list-backed values:" << std::endl;
    for (MutantStack<int, std::list<int> >::iterator it = listStack.begin();
         it != listStack.end(); ++it)
        std::cout << *it << std::endl;
    return 0;
}
