#include "Zombie.hpp"

void randomChump( std::string name ) {
    Zombie zombie(name);  //　ここで自分のnameのインスタンスを作成

    // nameを指定していなくても、インスタンスが自分のものなのでOK
    // announce()では、this->nameでメンバ変数を使用
    zombie.announce();
}
//　ここでZombie classのデストラクタが呼ばれる
