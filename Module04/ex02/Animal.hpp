#ifndef ANIMAL_HPP
# define ANIMAL_HPP

# include <string>

// Animalをインスタンス化しない

class Animal {

protected:
	std::string type;

public:
	// OCF
	Animal();
    Animal(const Animal& src);
    Animal& operator=(const Animal& rhs);
	virtual ~Animal();

	// 純粋仮想関数 -> 抽象クラス -> new Animal()ができない
	virtual void makeSound() const = 0;

	std::string getType() const;
};

#endif

// 抽象クラス: 土台用のクラス
// 純粋仮想関数: 関数の名前と形だけを定義した関数(= 0) -> 子クラスでのオーバーライド必須


