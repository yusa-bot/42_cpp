#ifndef ANIMAL_HPP
# define ANIMAL_HPP

# include <string>

// Virtualの動的結合

class Animal {

protected:
	std::string type;

public:
	// OCF
	Animal();
    Animal(const Animal& src);
    Animal& operator=(const Animal& rhs);
	virtual ~Animal(); // Vtableが指す別のデストラクタを実行

	virtual void makeSound() const; // 仮想関数
	std::string getType() const;
};

#endif

// ex.)
// Animal* j = new Dog();
// j->makeSound();

// Virtualなし: Animal型のため、Animal::makeSound() を呼び出す
// Virtualあり: 動的結合
//	1. Vtableを親/子ともに持つ
//	2. インスタンス生成時、Vptr(Vtableのptr)を持つ
//	3. 実行時、インスタンスのVptrを辿り、Dog 用の Vtable を参照しアドレスを特定して実行

