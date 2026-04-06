#ifndef DOG_HPP
# define DOG_HPP

# include "Animal.hpp"

class Dog: public Animal {

public:
	// OCF
	Dog();
	Dog(const Dog& src);
	Dog& operator=(const Dog& rhs);
    ~Dog();

	void makeSound() const;
};

#endif
