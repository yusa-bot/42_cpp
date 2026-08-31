#include "PmergeMe.hpp"

#include <unistd.h>

int main() {
    char program[] = "PmergeMe";
    char one[] = "9";
    char two[] = "3";
    char three[] = "7";
    char four[] = "1";
    char five[] = "5";
    char *arguments[] = {program, one, two, three, four, five};
    {
        PmergeMe first;
        first.process(6, arguments);
        PmergeMe second(first);
        PmergeMe third;
        third = second;
    }
    sleep(4);
    return 0;
}
