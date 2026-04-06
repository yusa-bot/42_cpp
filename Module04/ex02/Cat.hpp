#ifndef CAT_HPP
# define CAT_HPP

# include "Animal.hpp"
# include "Brain.hpp"

class Cat: public Animal {

private:
	Brain* _brain;

public:
	// OCF
	Cat();
	Cat(const Cat& src);
	Cat& operator=(const Cat& rhs);
    ~Cat();

	void makeSound() const;

	// デバッグ用
	void setIdea(int index, const std::string& idea);
    std::string getIdea(int index) const;
};

#endif
