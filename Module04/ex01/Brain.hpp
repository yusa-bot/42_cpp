#ifndef BRAIN_HPP
# define BRAIN_HPP

# include <string>

class Brain {

private:
	std::string ideas[100];

public:
	// OCF
	Brain();
    Brain(const Brain& src);
    Brain& operator=(const Brain& rhs);
    ~Brain();

	void setIdea(int index, const std::string& idea);
    std::string getIdea(int index) const;
};

#endif
