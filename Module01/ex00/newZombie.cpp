#include "Zombie.hpp"

Zombie* newZombie( std::string name ) {
    // new: *を返す -> Zombie class のコンストラクタ呼ぶ
    return new Zombie(name);
}
// -> Zombieインスタンスを作成 -> Zombie class の コンストラクタが呼ばれる
