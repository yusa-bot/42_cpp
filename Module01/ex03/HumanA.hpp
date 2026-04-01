#ifndef HUMAN_A_HPP
# define HUMAN_A_HPP

# include <string>
# include "Weapon.hpp"

class HumanA {

private:
    std::string _name;

    // classWeaponのtypeの別名
    Weapon& _weapon; // 参照のため、NULL無し

public:
    HumanA(std::string name, Weapon& weapon);
    ~HumanA();

    // 以下のように（角括弧なしで）表示します：<name> が <weapon type> で攻撃する
    void attack(void) const;
};

#endif
