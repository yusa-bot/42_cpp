#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include <iostream>

static void title(const char* s){ std::cout << "\n### " << s << std::endl; }

int main() {
    title("Bureaucrat bounds grade 1 and 150 valid");
    try { Bureaucrat a("hi",1); Bureaucrat b("lo",150); std::cout << a << " | " << b << std::endl; }
    catch (std::exception& e){ std::cout << "UNEXPECTED throw: " << e.what() << std::endl; }

    title("increment at grade 1 must throw (too high)");
    try { Bureaucrat a("x",1); a.incrementGrade(); std::cout << "NO THROW (BUG)" << std::endl; }
    catch (std::exception& e){ std::cout << "threw: " << e.what() << std::endl; }

    title("decrement at grade 150 must throw (too low)");
    try { Bureaucrat a("x",150); a.decrementGrade(); std::cout << "NO THROW (BUG)" << std::endl; }
    catch (std::exception& e){ std::cout << "threw: " << e.what() << std::endl; }

    title("beSigned at EXACT required grade must succeed (grade==gradeToSign)");
    try {
        Bureaucrat a("exact",145);
        ShrubberyCreationForm f("home"); // sign 145 exec 137
        a.signForm(f);
        std::cout << f << std::endl;
    } catch (std::exception& e){ std::cout << "UNEXPECTED: " << e.what() << std::endl; }

    title("execute UNSIGNED must throw FormNotSigned");
    try {
        Bureaucrat a("chief",1);
        ShrubberyCreationForm f("home");
        f.execute(a);
        std::cout << "NO THROW (BUG)" << std::endl;
    } catch (std::exception& e){ std::cout << "threw: " << e.what() << std::endl; }

    title("execute at EXACT exec grade boundary must succeed (137)");
    try {
        Bureaucrat signer("signer",145);
        Bureaucrat exec("exec",137);
        ShrubberyCreationForm f("exactexec");
        signer.signForm(f);
        exec.executeForm(f);
    } catch (std::exception& e){ std::cout << "UNEXPECTED: " << e.what() << std::endl; }

    title("execute one below exec grade (138) must be refused");
    try {
        Bureaucrat signer("signer",145);
        Bureaucrat weak("weak",138);
        ShrubberyCreationForm f("weakexec");
        signer.signForm(f);
        weak.executeForm(f);
    } catch (std::exception& e){ std::cout << "threw: " << e.what() << std::endl; }

    std::remove("home_shrubbery");
    std::remove("exactexec_shrubbery");
    return 0;
}
