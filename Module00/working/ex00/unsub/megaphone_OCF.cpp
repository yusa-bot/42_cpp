#include <iostream>
#include <cctype>

// -----------------------------------------------------

class Megaphone {
public:
    // Orthodox Canonical Form (正統的標準形式)
    Megaphone();
    Megaphone(const Megaphone &other);
    Megaphone &operator=(const Megaphone &other);
    ~Megaphone();

    void amplify(int argc, char **argv) const;
};

Megaphone::Megaphone() {}
Megaphone::Megaphone(const Megaphone &other) {
    (void)other;
}
Megaphone &Megaphone::operator=(const Megaphone &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}
Megaphone::~Megaphone() {}

// -----------------------------------------------------

void Megaphone::amplify(int argc, char **argv) const { // const: クラスのメンバ変数を書き換えない

    if (argc == 1) {
        std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *" << std::endl;
        return;
    }

    for (int i = 1; i < argc; ++i) {
        for (int j = 0; argv[i][j] != '\0'; ++j) {
            std::cout << static_cast<char>(std::toupper(argv[i][j]));
        }
    }
    std::cout << std::endl;
}

int main(int argc, char **argv) {
    Megaphone megaphone;
    megaphone.amplify(argc, argv);
    return 0;
}
