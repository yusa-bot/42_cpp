#ifndef ZOMBIE_HPP
# define ZOMBIE_HPP

# include <string>

// stack, heap の違い

class Zombie {

private:
	std::string name;

public:
	Zombie(std::string name);
	~Zombie(); // デストラクタは、デバッグ目的でゾンビの名前を含むメッセージを出力する必要

	void announce( void );
};

// この関数はゾンビを作成し、名前を付け、関数のスコープ外でも使用できるようにそれを返します。
// new -> Heap: 関数のスコープ外でも使用 -> delete
Zombie* newZombie( std::string name );

// この関数はゾンビを作成し、名前を付け、自分自身をアナウンスさせます(announce())
// Stack: 関数の中だけ->消滅
void randomChump( std::string name );

#endif