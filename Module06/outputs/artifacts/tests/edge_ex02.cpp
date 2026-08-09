#include "Base.hpp"
#include <iostream>
int main() {
    std::cout << "== identify(Base*) with NULL (must not crash) ==" << std::endl;
    identify((Base*)NULL);
    std::cout << "survived NULL pointer" << std::endl;

    std::cout << "== generate()+identify loop leak-safety ==" << std::endl;
    for (int i = 0; i < 5; ++i) {
        Base* b = generate();
        std::cout << "gen: "; identify(b); identify(*b);
        delete b;
    }
    return 0;
}
