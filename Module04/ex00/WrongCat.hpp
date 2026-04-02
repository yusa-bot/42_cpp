#ifndef WRONGCAT_HPP
# define WRONGCAT_HPP

# include "WrongAnimal.hpp"

class WrongCat: public WrongAnimal {

public:
	// OCF
	WrongCat();
	WrongCat(const std::string& type);
	WrongCat(const WrongCat& src);
	WrongCat& operator=(const WrongCat& rhs);
    ~WrongCat();

	void makeSound() const;
};

#endif
