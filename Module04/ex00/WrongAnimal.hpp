#ifndef WRONGANIMAL_HPP
# define WRONGANIMAL_HPP

# include <string>

class WrongAnimal {

protected:
	std::string type;

public:
	// OCF
	WrongAnimal();
    WrongAnimal(const WrongAnimal& src);
    WrongAnimal& operator=(const WrongAnimal& rhs);
	~WrongAnimal();

	void makeSound() const;
	std::string getType() const;
};

#endif
