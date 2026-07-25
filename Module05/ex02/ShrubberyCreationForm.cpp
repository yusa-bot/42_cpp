#include "ShrubberyCreationForm.hpp"
#include <fstream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

ShrubberyCreationForm::ShrubberyCreationForm(const std::string& target)
    : AForm("ShrubberyCreationForm", 145, 137), _target(target) {
}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm& src)
    : AForm(src), _target(src._target) {
}

ShrubberyCreationForm& ShrubberyCreationForm::operator=(const ShrubberyCreationForm& rhs) {
    if (this != &rhs)
        AForm::operator=(rhs);
    return *this;
}

ShrubberyCreationForm::~ShrubberyCreationForm() {
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

void ShrubberyCreationForm::executeAction() const {
    std::ofstream file((this->_target + "_shrubbery").c_str());

    if (!file)
        throw FileOpenException();

    file << "       /\\\n";
    file << "      /**\\\n";
    file << "     /****\\\n";
    file << "    /******\\\n";
    file << "   /********\\\n";
    file << "       ||\n";
    file << "       ||\n";
}

// -----------------------------------------------------------------------------
// 例外クラス
// -----------------------------------------------------------------------------

const char* ShrubberyCreationForm::FileOpenException::what() const throw() {
    return "could not open shrubbery file";
}
