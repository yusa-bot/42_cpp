#ifndef CAT_HPP
# define CAT_HPP

# include "Animal.hpp"

class Cat: public Animal {

public:
	// OCF
	Cat();
	Cat(const Cat& src);
	Cat& operator=(const Cat& rhs);
    ~Cat();

	void makeSound() const;
};

#endif
