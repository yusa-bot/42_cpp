#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"

#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>

Base::~Base() {
}

Base* generate(void) {
    static bool seeded = false;

    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(NULL)));
        seeded = true;
    }
    switch (std::rand() % 3) {
        case 0:
            return new A;
        case 1:
            return new B;
        default:
            return new C;
    }
}

void identify(Base* p) {
    if (dynamic_cast<A*>(p))
        std::cout << "A" << std::endl;
    else if (dynamic_cast<B*>(p))
        std::cout << "B" << std::endl;
    else if (dynamic_cast<C*>(p))
        std::cout << "C" << std::endl;
}

void identify(Base& p) {
    try {
        (void)dynamic_cast<A&>(p);
        std::cout << "A" << std::endl;
        return;
    } catch (const std::exception&) {
    }
    try {
        (void)dynamic_cast<B&>(p);
        std::cout << "B" << std::endl;
        return;
    } catch (const std::exception&) {
    }
    try {
        (void)dynamic_cast<C&>(p);
        std::cout << "C" << std::endl;
    } catch (const std::exception&) {
    }
}
