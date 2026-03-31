#ifndef PHONEBOOK_HPP
# define PHONEBOOK_HPP

# include "Contact.hpp"

// It has an array of contacts.
class PhoneBook {

private:
    Contact contacts[8];
    int currentIndex;
    int totalContacts;

public:
    PhoneBook();

    bool addContact();
    bool searchContact() const;
};

#endif

//PhoneBook: プログラム全体の操作
//Contact: indexに該当するContactの操作
//-> PhoneBookで処理をしつつ、個々のContactはContactで処理。
//(Contact自体はPhoneBookがインスタンスを保持)

//////

//private : メンバ関数からのみアクセス -> 値の整合性を保つ
//public : main関数等からアクセス -> 外部から操作する

//.hpp : classの中身を教える -> メモリ容量を確定
