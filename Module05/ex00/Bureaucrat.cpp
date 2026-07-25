#include "Bureaucrat.hpp"

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

Bureaucrat::Bureaucrat() : _name("default"), _grade(150) {
}

Bureaucrat::Bureaucrat(const Bureaucrat& src) : _name(src._name), _grade(src._grade) {
}

Bureaucrat& Bureaucrat::operator=(const Bureaucrat& rhs) {
    // _nameはconstのため代入できない -> _gradeのみコピー
    if (this != &rhs) {
        this->_grade = rhs._grade;
    }
    return *this;
}

Bureaucrat::~Bureaucrat() {
}

Bureaucrat::Bureaucrat(const std::string& name, int grade) : _name(name) {
    if (grade < 1)
        throw Bureaucrat::GradeTooHighException();
    if (grade > 150)
        throw Bureaucrat::GradeTooLowException();
    this->_grade = grade;
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

std::string Bureaucrat::getName() const {
    return this->_name;
}

int Bureaucrat::getGrade() const {
    return this->_grade;
}

void Bureaucrat::incrementGrade() {
    if (this->_grade - 1 < 1)
        throw Bureaucrat::GradeTooHighException();
    this->_grade--;
}

void Bureaucrat::decrementGrade() {
    if (this->_grade + 1 > 150)
        throw Bureaucrat::GradeTooLowException();
    this->_grade++;
}

// -----------------------------------------------------------------------------
// 例外クラス
// -----------------------------------------------------------------------------

const char* Bureaucrat::GradeTooHighException::what() const throw() {
    return "grade is too high";
}

const char* Bureaucrat::GradeTooLowException::what() const throw() {
    return "grade is too low";
}

// -----------------------------------------------------------------------------
// 挿入演算子
// -----------------------------------------------------------------------------

// ex.) std::cout << bureaucrat; の場合 引数は、os: std::cout, rhs: bureaucrat
// osは内部バッファへの仲介口
// 返り値std::ostream& は、再びoperator<<を呼んで、連続で<<を使えるようにするため
std::ostream& operator<<(std::ostream& os, const Bureaucrat& rhs) {
    os << rhs.getName() << ", bureaucrat grade " << rhs.getGrade() << ".";
    return os;
}
