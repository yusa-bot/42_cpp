#include "PresidentialPardonForm.hpp"
#include <iostream>

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

PresidentialPardonForm::PresidentialPardonForm(const std::string& target)
    : AForm("PresidentialPardonForm", 25, 5), _target(target) {
}

PresidentialPardonForm::PresidentialPardonForm(const PresidentialPardonForm& src)
    : AForm(src), _target(src._target) {
}

PresidentialPardonForm& PresidentialPardonForm::operator=(const PresidentialPardonForm& rhs) {
    if (this != &rhs)
        AForm::operator=(rhs);
    return *this;
}

PresidentialPardonForm::~PresidentialPardonForm() {
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

void PresidentialPardonForm::executeAction() const {
    std::cout << this->_target
              << " has been pardoned by Zaphod Beeblebrox"
              << std::endl;
}
