#include "AForm.hpp"
#include "Bureaucrat.hpp"
#include <ostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

AForm::AForm()
    : _name("default"), _signed(false),
      _gradeToSign(150), _gradeToExecute(150) {}

AForm::AForm(const AForm& src)
    : _name(src._name), _signed(src._signed),
      _gradeToSign(src._gradeToSign),
      _gradeToExecute(src._gradeToExecute) {}

AForm& AForm::operator=(const AForm& rhs) {
    if (this != &rhs)
        this->_signed = rhs._signed;
    return *this;
}

AForm::~AForm() {}

// gradeToSign等が不正の値の場合、初期化後にthrowしても、AFormオブジェクトの生成は失敗するので良い。
// stringとintでメモリの大きさが違うので渡し方が違う
AForm::AForm(const std::string& name, int gradeToSign, int gradeToExecute)
    : _name(name), _signed(false), _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute) {
    if (gradeToSign < 1 || gradeToExecute < 1)
        throw GradeTooHighException();
    if (gradeToSign > 150 || gradeToExecute > 150)
        throw GradeTooLowException();
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

std::string AForm::getName() const {
    return this->_name;
}

bool AForm::getIsSigned() const {
    return this->_signed;
}

int AForm::getGradeToSign() const {
    return this->_gradeToSign;
}

int AForm::getGradeToExecute() const {
    return this->_gradeToExecute;
}

void AForm::beSigned(const Bureaucrat& bureaucrat) {
    if (bureaucrat.getGrade() > this->_gradeToSign)
        throw GradeTooLowException();
    this->_signed = true;
}

void AForm::execute(const Bureaucrat& executor) const {
    if (!this->_signed)
        throw FormNotSignedException();
    if (executor.getGrade() > this->_gradeToExecute)
        throw GradeTooLowException();
    this->executeAction();
}

// -----------------------------------------------------------------------------
// 例外クラス
// -----------------------------------------------------------------------------

const char* AForm::GradeTooHighException::what() const throw() {
    return "form grade is too high";
}

const char* AForm::GradeTooLowException::what() const throw() {
    return "form grade is too low";
}

const char* AForm::FormNotSignedException::what() const throw() {
    return "form is not signed";
}

// -----------------------------------------------------------------------------
// 挿入演算子
// -----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const AForm& rhs) {
    os << rhs.getName()
       << ", signed: " << (rhs.getIsSigned() ? "true" : "false")
       << ", grade to sign: " << rhs.getGradeToSign()
       << ", grade to execute: " << rhs.getGradeToExecute();
    return os;
}
