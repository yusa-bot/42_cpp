#include "Contact.hpp"
#include <iostream>
#include <iomanip>

Contact::Contact() {}

Contact::Contact(const Contact &other) {
    *this = other;
}

// this : クラスのメンバ変数を操作
Contact& Contact::operator=(const Contact &other) {
    if (this != &other) {
        this->firstName = other.firstName;
        this->lastName = other.lastName;
        this->nickname = other.nickname;
        this->phoneNumber = other.phoneNumber;
        this->darkestSecret = other.darkestSecret;
    }
    return *this;
}

// -----------------------------------------------------

std::string Contact::truncate(std::string str) const {
    if (str.length() > 10) {
        return str.substr(0, 9) + ".";
    }
    return str;
}

// -----------------------------------------------------

// ユーザーからの入力を受け取る
bool Contact::init() {
    std::string input[5];
    std::string prompts[5] = {
        "First Name: ",
        "Last Name: ",
        "Nickname: ",
        "Phone Number: ",
        "Darkest Secret: "
    };

    // 入力
    for (int i = 0; i < 5; ++i) {
        input[i] = "";

        // 空の入力を弾く（スペースのみの入力も防ぐ場合はさらに工夫が必要ですが、ここでは長さで判定）
        while (input[i].empty()) {
            std::cout << prompts[i];

            if (!std::getline(std::cin, input[i])) {
                return false; // EOF (Ctrl+D) などが入力された場合は終了
            }
        }
    }

    this->firstName = input[0];
    this->lastName = input[1];
    this->nickname = input[2];
    this->phoneNumber = input[3];
    this->darkestSecret = input[4];

    return true;
}

// ----------------------

// 一覧の表示
void Contact::viewRow(int index) const {
    std::cout << std::setw(10) << std::right << index << "|"
              << std::setw(10) << std::right << this->truncate(this->firstName) << "|"
              << std::setw(10) << std::right << this->truncate(this->lastName) << "|"
              << std::setw(10) << std::right << this->truncate(this->nickname) << std::endl;
}

// ----------------------

void Contact::viewFull() const {
    std::cout << "First Name: " << this->firstName << std::endl;
    std::cout << "Last Name: " << this->lastName << std::endl;
    std::cout << "Nickname: " << this->nickname << std::endl;
    std::cout << "Phone Number: " << this->phoneNumber << std::endl;
    std::cout << "Darkest Secret: " << this->darkestSecret << std::endl;
}
