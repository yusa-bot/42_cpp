#include "Zombie.hpp"
#include <iostream>

int main()
{
    int N = 5;
    std::string hordeName = "Walker";

    std::cout << "--- Creating a Zombie Horde of " << N << " zombies ---" << std::endl;
    Zombie* horde = zombieHorde(N, hordeName);

    if (horde == NULL) {
        std::cout << "Horde creation failed." << std::endl;
        return 1;
    }

    std::cout << "--- Announcing the Zombie Horde ---" << std::endl;
    for (int i = 0; i < N; i++) {
        horde[i].announce();
    }

    std::cout << "--- Destroying the Zombie Horde ---" << std::endl;
    delete[] horde;

    return 0;
}
