#include "RobotomyRequestForm.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

// 関数そのものを、この .cpp ファイル以外から見えなくする
namespace {
bool robotomySucceeded() {
    static bool seeded = false;

    if (!seeded) {
        // プログラム1回につき任意のseedを設定 (プログラムごとに一連の結果を変える)
        std::srand(static_cast<unsigned int>(std::time(NULL)));
        seeded = true;
    }
    // そのseedを元にランダム生成
    return std::rand() % 2 == 0;
}
}

// -----------------------------------------------------------------------------
// OCF
// -----------------------------------------------------------------------------

RobotomyRequestForm::RobotomyRequestForm(const std::string& target)
    : AForm("RobotomyRequestForm", 72, 45), _target(target) {
}

RobotomyRequestForm::RobotomyRequestForm(const RobotomyRequestForm& src)
    : AForm(src), _target(src._target) {
}

RobotomyRequestForm& RobotomyRequestForm::operator=(const RobotomyRequestForm& rhs) {
    if (this != &rhs)
        AForm::operator=(rhs);
    return *this;
}

RobotomyRequestForm::~RobotomyRequestForm() {
}

// -----------------------------------------------------------------------------
// メンバ関数
// -----------------------------------------------------------------------------

void RobotomyRequestForm::executeAction() const {
    std::cout << "Bzzzzzz... drilling noises..." << std::endl;
    if (robotomySucceeded())
        std::cout << this->_target << " has been robotomized successfully" << std::endl;
    else
        std::cout << "Robotomy failed for " << this->_target << std::endl;
}
