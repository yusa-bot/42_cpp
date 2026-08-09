#ifndef EASYFIND_HPP
# define EASYFIND_HPP

# include <algorithm>
# include <stdexcept>

// Key Point: container, iterator

template <typename T>
typename T::iterator easyfind(T& container, int value) {
    typename T::iterator found = std::find(
        container.begin(), container.end(), value);
    if (found == container.end())
        throw std::out_of_range("value was not found in the container");
    return found;
}

// containerがconstかとは別に、「そのイテレータ経由のアクセスを読み取り専用にするか」を型として表すために const_iterator がある
template <typename T>
typename T::const_iterator easyfind(const T& container, int value) {
    typename T::const_iterator found = std::find(
        container.begin(), container.end(), value); // find()はconst containerのためconst_iteratorを返す
    if (found == container.end())
        throw std::out_of_range("value was not found in the container");
    return found;
}

#endif
