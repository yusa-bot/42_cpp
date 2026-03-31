class Zombie {

private:
	std::string name;

	void announce( void );

	// この関数はゾンビを作成し、名前を付け、自分自身をアナウンスさせます
	void randomChump( std::string name );

public:
	Zombie();
	// デストラクタは、デバッグ目的でゾンビの名前を含むメッセージを出力する必要
	~Zombie();

	// この関数はゾンビを作成し、名前を付け、関数のスコープ外でも使用できるようにそれを返します。
	Zombie* newZombie( std::string name );

}

// Foo: BraiiiiiiinnnzzzZ...

ゾンビをスタック上に割り当てるべきか、
ヒープ上に割り当てるべきかを判断する必要

ゾンビは不要になった時点で破棄