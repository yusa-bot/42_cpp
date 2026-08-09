#include "Intern.hpp"
#include "AForm.hpp"
#include <iostream>
int main() {
    Intern in;
    const char* names[] = {"shrubbery creation","robotomy request","presidential pardon","coffee request",""};
    for (int i=0;i<5;i++){
        AForm* f = in.makeForm(names[i], "tgt");
        std::cout << "  -> returned " << (f? "NON-NULL":"NULL") << std::endl;
        if (f) { std::cout << "     " << *f << std::endl; delete f; }
    }
    return 0;
}
