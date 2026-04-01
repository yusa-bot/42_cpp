#ifndef WEAPON_HPP
# define WEAPON_HPP

# include <string>

// typeを保持,操作する
class Weapon {

private:
    std::string type;

public:
    Weapon(std::string type);
    ~Weapon();

    // type への定数参照を返すメンバ関数
    const std::string& getType(void) const;

    // パラメータとして渡された新しい値を使用して type を設定するメンバ関数。
    void setType(std::string type);
};

#endif
