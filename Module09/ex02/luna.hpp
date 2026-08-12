#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <deque>
# include <string>
# include <vector>

class PmergeMe {
public:
    static std::vector<int> parseInput(int argc, char **argv);
    static void sortVector(std::vector<int>& values);
    static void sortDeque(std::deque<int>& values);
    static void print(const std::string& label,
                      const std::vector<int>& values);

private:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();
};

#endif

// -03
