#include "Bureaucrat.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include <iostream>

int main() {
    Bureaucrat chief("Chief", 1);
    Bureaucrat clerk("Clerk", 140);
    Bureaucrat intern("Intern", 150);
    ShrubberyCreationForm garden("garden");
    RobotomyRequestForm robotomy("Marvin");
    PresidentialPardonForm pardon("Arthur");

    std::cout << "===== unsigned form =====" << std::endl;
    chief.executeForm(garden);

    std::cout << "\n===== signing rejected =====" << std::endl;
    intern.signForm(pardon);

    std::cout << "\n===== execution rejected: grade too low =====" << std::endl;
    clerk.signForm(garden);
    clerk.executeForm(garden);

    std::cout << "\n===== ShrubberyCreationForm =====" << std::endl;
    chief.executeForm(garden); //　別のBureaucratのsignでも良い

    std::cout << "\n===== RobotomyRequestForm =====" << std::endl;
    chief.signForm(robotomy);
    chief.executeForm(robotomy);

    std::cout << "\n===== PresidentialPardonForm =====" << std::endl;
    chief.signForm(pardon);
    chief.executeForm(pardon);

    return 0;
}
