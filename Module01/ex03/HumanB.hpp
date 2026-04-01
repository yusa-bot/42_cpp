#ifndef HUMAN_B_HPP
# define HUMAN_B_HPP

# include <string>
# include "Weapon.hpp"

class HumanB {

private:
    std::string _name;

    // Weaponのtypeのアドレス
    Weapon* _weapon;  // ポインタのためNULL有り

public:
    HumanB(std::string name);
    ~HumanB();

    // 以下のように（角括弧なしで）表示します：<name> が <weapon type> で攻撃する
    void attack(void) const;

    void setWeapon(Weapon& weapon);
};

#endif
