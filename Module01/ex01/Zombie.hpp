#ifndef ZOMBIE_HPP
# define ZOMBIE_HPP

# include <string>

class Zombie {

private:
	std::string name;

public:
	~Zombie();

	void announce( void );

	// 名前を設定する関数を追加
	void setName(std::string name);
};

// この関数は、一度の割り当てで N 個の Zombie オブジェクトを割り当てる
// 各ゾンビにパラメータとして渡された名前を割り当てて初期化
// 最初のゾンビへのポインタを返します
Zombie* zombieHorde( int N, std::string name );

#endif
