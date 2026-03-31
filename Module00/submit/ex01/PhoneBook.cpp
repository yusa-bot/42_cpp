#include "PhoneBook.hpp"
#include <iostream>
#include <iomanip>

PhoneBook::PhoneBook() : currentIndex(0), totalContacts(0) {}

// -----------------------------------------------------

bool PhoneBook::addContact() {
    Contact newContact;

    // ユーザーからの入力を受け取る
    if (!newContact.init()) {
        return false; // EOF検出時
    }

    // 古いものを上書きするロジック (currentIndex は 0〜7 をループする)
    this->contacts[this->currentIndex] = newContact;
    this->currentIndex = (this->currentIndex + 1) % 8;

    if (this->totalContacts < 8) {
        this->totalContacts++;
    }

    std::cout << "Contact added successfully!" << std::endl;
    return true;
}

bool PhoneBook::searchContact() const {

    if (this->totalContacts == 0) {
        std::cout << "The phonebook is empty." << std::endl;
        return true;
    }

    // ヘッダーの表示
    std::cout << std::setw(10) << std::right << "Index" << "|"
              << std::setw(10) << std::right << "First Name" << "|"
              << std::setw(10) << std::right << "Last Name" << "|"
              << std::setw(10) << std::right << "Nickname" << std::endl;

    // 一覧の表示
    for (int i = 0; i < this->totalContacts; ++i) {
        this->contacts[i].viewRow(i);
    }

    // インデックスの入力要求
    std::string indexStr;
    std::cout << "Enter index to view details: ";
    if (!std::getline(std::cin, indexStr)) {
        return false; // EOF検出時
    }

    // 入力されたインデックスの検証
    // 1文字かつ、'0'から総数未満の数字であるかをチェック
    if (indexStr.length() == 1 && indexStr[0] >= '0' && indexStr[0] < '0' + this->totalContacts) {
        int idx = indexStr[0] - '0';
        this->contacts[idx].viewFull();
    } else {
        std::cout << "Invalid index!" << std::endl;
    }
    return true;
}
