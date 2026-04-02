#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "WrongAnimal.hpp"
#include "WrongCat.hpp"
#include <iostream>

int main()
{
	std::cout << "--- Standard Animal (Virtual) ---" << std::endl;
	const Animal* meta = new Animal();
	const Animal* j = new Dog();
	const Animal* i = new Cat();

	std::cout << j->getType() << " " << std::endl;
	std::cout << i->getType() << " " << std::endl;

	i->makeSound();
	j->makeSound();
	meta->makeSound();

	std::cout << "\n--- Deleting ---" << std::endl;
    delete meta;
    delete j;
    delete i;

	std::cout << "\n--- Wrong Animal (Non-Virtual) ---" << std::endl;
	const WrongAnimal* wrongCat = new WrongCat();
	wrongCat->makeSound();

	delete wrongCat;
	return 0;
}
