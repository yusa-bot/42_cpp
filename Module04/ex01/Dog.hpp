#ifndef DOG_HPP
# define DOG_HPP

# include "Animal.hpp"
# include "Brain.hpp"

class Dog: public Animal {

private:
	Brain* _brain;

public:
	// OCF
	Dog();
	Dog(const Dog& src);
	Dog& operator=(const Dog& rhs);
    ~Dog();

	void makeSound() const;

	// デバッグ用
	void setIdea(int index, const std::string& idea);
    std::string getIdea(int index) const;
};

#endif
