#include <string>
#include <iostream>

// 参照の基礎を学ぶex

int main() {
    std::string str = "HI THIS IS BRAIN";

    std::string* stringPTR = &str;

    // 参照(別名)を作成
    std::string& stringREF = str;

    // memory
    std::cout << "The memory address of the string variable: " << &str << std::endl;
    std::cout << "The memory address held by stringPTR: " << stringPTR << std::endl;
    std::cout << "The memory address held by stringREF: " << &stringREF << std::endl; // strと同じように&

    // value
    std::cout << "\nThe value of the string variable: " << str << std::endl;
    std::cout << "The value pointed to by stringPTR: " << *stringPTR << std::endl;
    std::cout << "The value pointed to by stringREF: " << stringREF << std::endl; // strと同じようにそのまま

}

// 参照はただの別名。ポインタのようには使わない

// 宣言時の初期化が必須, 再代入不可
// -> C言語のポインタの進化版でバグを防ぐ
