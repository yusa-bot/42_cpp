#ifndef FRAGCRAP_HPP
# define FRAGCRAP_HPP

# include "ClapTrap.hpp"

class FragTrap: public ClapTrap {

public:
	// OCF
	FragTrap();
	FragTrap(const std::string& name);
	FragTrap(const FragTrap& src);
	FragTrap& operator=(const FragTrap& rhs);
    ~FragTrap();

	void highFivesGuys(void);
};

#endif
