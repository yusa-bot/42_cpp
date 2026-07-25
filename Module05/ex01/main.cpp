#include "Bureaucrat.hpp"
#include "Form.hpp"
#include <iostream>

int main() {
    std::cout << "===== successful signing =====" << std::endl;
    try {
        Bureaucrat alice("Alice", 42);
        Form leaveRequest("LeaveRequest", 50, 25);

        std::cout << alice << std::endl;
        std::cout << leaveRequest << std::endl;
        alice.signForm(leaveRequest);
        std::cout << leaveRequest << std::endl;
    }
    catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    std::cout << "\n===== signing rejected: grade too low =====" << std::endl;
    try {
        Bureaucrat bob("Bob", 75);
        Form budgetRequest("BudgetRequest", 50, 25);

        std::cout << budgetRequest << std::endl;
        bob.signForm(budgetRequest);
        std::cout << budgetRequest << std::endl;
    }
    catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    std::cout << "\n===== invalid Form grades =====" << std::endl;
    try {
        Form tooHigh("TooHigh", 0, 25);
        std::cout << tooHigh << std::endl;
    }
    catch (std::exception& e) {
        std::cout << "grade 0: " << e.what() << std::endl;
    }

    try {
        Form tooLow("TooLow", 25, 151);
        std::cout << tooLow << std::endl;
    }
    catch (std::exception& e) {
        std::cout << "grade 151: " << e.what() << std::endl;
    }

    return 0;
}
