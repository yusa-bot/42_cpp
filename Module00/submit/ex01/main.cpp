#include "PhoneBook.hpp"
#include <iostream>
#include <string>

int main() {

    // PhoneBook.hppをcheck & PhoneBook::PhoneBook()
    PhoneBook phoneBook;

    std::string command;

    std::cout << "Welcome to the 80s PhoneBook!" << std::endl;

    while (true) {
        std::cout << "> Enter command (ADD, SEARCH, EXIT): ";

        // std::getline で読み込み、EOF (Ctrl+D) が来たら安全に終了する
        if (!std::getline(std::cin, command)) {
            std::cout << "\nEOF detected. Exiting program." << std::endl;
            break;
        }

        if (command == "ADD") {
            if (!phoneBook.addContact()) break;
        } else if (command == "SEARCH") {
            if (!phoneBook.searchContact()) break;
        } else if (command == "EXIT") {
            std::cout << "Goodbye! Contacts lost forever." << std::endl;
            break;
        } else {
            continue;
        }
    }
    return 0;
}
