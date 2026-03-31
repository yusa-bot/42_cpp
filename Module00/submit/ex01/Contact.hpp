#ifndef CONTACT_HPP
# define CONTACT_HPP

# include <string>

class Contact {

private:
    std::string firstName;
    std::string lastName;
    std::string nickname;
    std::string phoneNumber;
    std::string darkestSecret;

    // 10文字を超える文字列を切り捨てて '.' を付ける内部用ヘルパー関数
    std::string truncate(std::string str) const;

public:
    // Orthodox Canonical Form
    Contact();
    Contact(const Contact &other);
    Contact &operator=(const Contact &other);

    bool init();
    void viewRow(int index) const;
    void viewFull() const;
};

#endif

//Contact::init() でinput[5]にそれぞれを入力し、メンバ変数に登録する。
//Contact::viewRow(), Contact::viewFull()では、登録されたメンバ変数から読み取る。
//-> どちらもインスタjンスのメンバ変数を経由している。
