#ifndef ZOMBIE_HPP
# define ZOMBIE_HPP

# include <string>

// Zombie* hordeが叫ぶ
// 複数要素(配列)のインスタンスはコンストラクタを呼ぶ時に引数(name)を指定できない
// -> 空のデフォルトコンストラクタ -> メンバ関数setName()

class Zombie {

private:
	std::string name;

public:
	~Zombie();

	void announce( void );

	void setName(std::string name);
};

// N 個の Zombie オブジェクトを割り当て -> 各ゾンビにパラメータとして渡された名前を割り当てて初期化 -> 最初のゾンビへのポインタを返します
Zombie* zombieHorde( int N, std::string name );

#endif
