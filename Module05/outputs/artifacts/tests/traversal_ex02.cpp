#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
int main() {
    Bureaucrat chief("chief", 1);
    // target に相対パスを与えると作業ディレクトリ外にファイル生成できる
    ShrubberyCreationForm f("/tmp/Module05_v2/sub/evil");
    chief.signForm(f);
    chief.executeForm(f);
    return 0;
}
