#ifndef ANIMAL_HPP
# define ANIMAL_HPP

# include <string>

// Animalを継承してBrainを持つ, シャローコピー/ディープコピー

class Animal {

protected:
	std::string type;

public:
	// OCF
	Animal();
    Animal(const Animal& src);
    Animal& operator=(const Animal& rhs);
	virtual ~Animal();

	virtual void makeSound() const;
	std::string getType() const;
};

#endif
