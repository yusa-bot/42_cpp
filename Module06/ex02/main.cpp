#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"

#include <iostream>

static void testIdentification(Base& value) {
    std::cout << "identify(Base*): ";
    identify(&value);
    std::cout << "identify(Base&): ";
    identify(value);
}

int main() {
    Base* generated = generate();

    std::cout << "Generated object" << std::endl;
    testIdentification(*generated);
    delete generated;

    A a;
    B b;
    C c;

    std::cout << "A object" << std::endl;
    testIdentification(a);
    std::cout << "B object" << std::endl;
    testIdentification(b);
    std::cout << "C object" << std::endl;
    testIdentification(c);
    return 0;
}
