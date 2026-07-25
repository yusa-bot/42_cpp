#include "Form.hpp"
#include "Bureaucrat.hpp"
#include <ostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

Form::Form()
    : _name("default"), _signed(false),
      _gradeToSign(150), _gradeToExecute(150) {}

Form::Form(const Form& src)
    : _name(src._name), _signed(src._signed),
      _gradeToSign(src._gradeToSign),
      _gradeToExecute(src._gradeToExecute) {}

Form& Form::operator=(const Form& rhs) {
    if (this != &rhs)
        this->_signed = rhs._signed;
    return *this;
}

Form::~Form() {}

// gradeToSign等が不正の値の場合、初期化後にthrowしても、Form オブジェクトの生成は失敗するので良い。
// stringとintでメモリの大きさが違うので渡し方が違う
Form::Form(const std::string& name, int gradeToSign, int gradeToExecute)
    : _name(name), _signed(false), _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute) {
    if (gradeToSign < 1 || gradeToExecute < 1)
        throw GradeTooHighException();
    if (gradeToSign > 150 || gradeToExecute > 150)
        throw GradeTooLowException();
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

std::string Form::getName() const {
    return this->_name;
}

bool Form::getIsSigned() const {
    return this->_signed;
}

int Form::getGradeToSign() const {
    return this->_gradeToSign;
}

int Form::getGradeToExecute() const {
    return this->_gradeToExecute;
}

void Form::beSigned(const Bureaucrat& bureaucrat) {
    if (bureaucrat.getGrade() > this->_gradeToSign)
        throw GradeTooLowException();
    this->_signed = true;
}

// -----------------------------------------------------------------------------
// 例外クラス
// -----------------------------------------------------------------------------

const char* Form::GradeTooHighException::what() const throw() {
    return "form grade is too high";
}

const char* Form::GradeTooLowException::what() const throw() {
    return "form grade is too low";
}

// -----------------------------------------------------------------------------
// 挿入演算子
// -----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Form& rhs) {
    os << rhs.getName()
       << ", signed: " << (rhs.getIsSigned() ? "true" : "false")
       << ", grade to sign: " << rhs.getGradeToSign()
       << ", grade to execute: " << rhs.getGradeToExecute();
    return os;
}
