#include "AForm.hpp"
#include "Bureaucrat.hpp"
#include "Intern.hpp"
#include <iostream>

int main() {
    Bureaucrat chief("Chief", 1);
    Bureaucrat clerk("Clerk", 140);
    Intern someRandomIntern;
    AForm* shrubbery;
    AForm* robotomy;
    AForm* pardon;
    AForm* unknown;

    std::cout << "===== Intern creates forms =====" << std::endl;
    shrubbery = someRandomIntern.makeForm("shrubbery creation", "garden");
    robotomy = someRandomIntern.makeForm("robotomy request", "Bender");
    pardon = someRandomIntern.makeForm("presidential pardon", "Arthur");

    std::cout << "\n===== Unknown form =====" << std::endl;
    unknown = someRandomIntern.makeForm("coffee request", "Office");
    if (unknown != 0)
        delete unknown;

    std::cout << "\n===== Shrubbery execution checks =====" << std::endl;
    if (shrubbery != 0) {
        chief.executeForm(*shrubbery);
        clerk.signForm(*shrubbery);
        clerk.executeForm(*shrubbery);
        chief.executeForm(*shrubbery);
    }

    std::cout << "\n===== RobotomyRequestForm =====" << std::endl;
    if (robotomy != 0) {
        chief.signForm(*robotomy);
        chief.executeForm(*robotomy);
    }

    std::cout << "\n===== PresidentialPardonForm =====" << std::endl;
    if (pardon != 0) {
        chief.signForm(*pardon);
        chief.executeForm(*pardon);
    }

    delete shrubbery;
    delete robotomy;
    delete pardon;
    return 0;
}
