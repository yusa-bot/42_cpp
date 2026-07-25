#include "Bureaucrat.hpp"
#include <iostream>

int main() {
    std::cout << "===== incrementGrade test =====" << std::endl;
    try {
        Bureaucrat alice("Alice", 2);
        // 2回<<を使うが、引数は1回目:alice, 2回目std::endlとなり、引数の型に応じて適切なオーバーロードが選択される
        std::cout << alice << std::endl;

        std::cout << "\n--- incrementGrade: 2 -> 1 ---" << std::endl;
        alice.incrementGrade();
        std::cout << alice << std::endl;

        std::cout << "\n--- More increment -> Exception ---" << std::endl;
        alice.incrementGrade();
        std::cout << "Not reach here." << std::endl;
    }
    catch (std::exception& e) {
        std::cout << "Exception catch: " << e.what() << std::endl;
    }

    std::cout << "\n\n===== decrementGrade test =====" << std::endl;
    try {
        Bureaucrat bob("Bob", 149);
        std::cout << bob << std::endl;

        std::cout << "\n--- More decrementGrade ---" << std::endl;
        bob.decrementGrade();
        std::cout << bob << std::endl;

        std::cout << "\n--- More decrementGrade -> Exception ---" << std::endl;
        bob.decrementGrade();
        std::cout << "Not reach here." << std::endl;
    }
    catch (std::exception& e) {
        std::cout << "Exception catch:  " << e.what() << std::endl;
    }

    std::cout << "\n\n===== Invalid grade instantiation =====" << std::endl;
    std::cout << "--- grade 0 ---" << std::endl;
    try {
        Bureaucrat invalid("TooHigh", 0);
    }
    catch (std::exception& e) {
        std::cout << "Exception catch: " << e.what() << std::endl;
    }

    std::cout << "\n--- grade 151 ---" << std::endl;
    try {
        Bureaucrat invalid("TooLow", 151);
    }
    catch (std::exception& e) {
        std::cout << "Exception catch: " << e.what() << std::endl;
    }

    std::cout << "\n\n===== copy test =====" << std::endl;
    Bureaucrat original("Original", 42);
    Bureaucrat copy(original);
    std::cout << copy << std::endl;

    Bureaucrat assigned;
    assigned = original; // コピー代入演算子 (_nameはconstなので_gradeのみコピーされる)
    std::cout << assigned << std::endl;

    return 0;
}
