#include "Zombie.hpp"

Zombie* zombieHorde( int N, std::string name ) {

    if (N <= 0) {
        return NULL;
    }

    Zombie* horde = new Zombie[N]; // 各要素のデフォルトコンストラクタZombie()がN回呼ばれる

    for (int i = 0; i < N; i++) {
        horde[i].setName(name);
    }

    return horde;
}

// 複数要素(配列)のインスタンスはコンストラクタを呼ぶ時に引数(name)を指定できない
// -> 空のデフォルトコンストラクタ -> メンバ関数setName()
