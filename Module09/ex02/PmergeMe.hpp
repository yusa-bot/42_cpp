#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <deque>
# include <vector>

// Key Point: 

class PmergeMe {
private:
    std::vector<int> _before;
    std::vector<int> _vectorResult;
    std::deque<int> _dequeResult;
    double _vectorTime;
    double _dequeTime;

    void swap(PmergeMe& other);

public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    void process(int argc, char **argv);
    void display() const;
};

#endif

// vector: メモリ連続している
// deque: メモリ連続していない, push_front()あり
