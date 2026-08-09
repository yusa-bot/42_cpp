#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
#include <iostream>
int main() {
    // target points into a non-existent directory -> ofstream open fails
    Bureaucrat chief("chief", 1);
    ShrubberyCreationForm f("/no_such_dir_12345/tree");
    chief.signForm(f);
    chief.executeForm(f); // must NOT crash; error handled via exception
    std::cout << "survived file-open failure without crash" << std::endl;
    return 0;
}
