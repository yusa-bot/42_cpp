#include <iostream>
#include <cctype>

class Megaphone {
public:
    void amplify(int argc, char **argv) const {
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
};

int main(int argc, char **argv) {
    Megaphone megaphone;
    megaphone.amplify(argc, argv);
    return 0;
}

//Megaphone::Megaphone() {}がない
//-> コンパイラが自動的に中身が空のデフォルトコンストラクタを勝手に作る
