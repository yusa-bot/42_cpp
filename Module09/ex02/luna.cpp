#include "PmergeMe.hpp"

#include <climits>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <stdexcept>

// -----------------------------------------------------------------------------
// 無名名前空間
// -----------------------------------------------------------------------------

namespace {

struct Pair {
    int small;
    int large;
};

template <typename Iterator>
Iterator lowerBound(Iterator first, Iterator last, int value) {
    while (first != last) {
        Iterator middle = first;
        typename std::iterator_traits<Iterator>::difference_type distance =
            std::distance(first, last);
        std::advance(middle, distance / 2);
        if (*middle < value) {
            first = middle;
            ++first;
        } else {
            last = middle;
        }
    }
    return first;
}

/*
 * Ford-Johnsonの挿入順:
 *
 *   b1, b3, b2, b5, b4, b11, b10, ...
 *
 * 各グループの終端はJacobsthal数になる。
 * この順番で挿入すると、二分探索の範囲をできるだけ小さくできる。
 */
std::vector<std::size_t> insertionOrder(std::size_t count) {
    std::vector<std::size_t> order;
    if (count == 0)
        return order;

    order.push_back(0); // b1は最初にa1の前へ挿入する
    std::size_t previous = 1;
    std::size_t current = 3;
    while (previous < count) {
        std::size_t end = current < count ? current : count;
        std::size_t index = end;
        while (index > previous) {
            order.push_back(index - 1); // 1始まりのb_iを添字へ変換する
            --index;
        }
        const std::size_t next = current + 2 * previous;
        previous = current;
        current = next;
    }
    return order;
}

template <typename Container>
void insertBeforeAnchor(Container& chain, int value, int anchor) {
    typename Container::iterator anchorIt = chain.begin();
    while (anchorIt != chain.end() && *anchorIt != anchor)
        ++anchorIt;

    typename Container::iterator position;
    if (anchorIt == chain.end())
        position = lowerBound(chain.begin(), chain.end(), value);
    else
        position = lowerBound(chain.begin(), anchorIt, value);
    chain.insert(position, value);
}

template <typename Container>
void fordJohnson(Container& values) {
    if (values.size() < 2)
        return;

    std::vector<Pair> pairs;
    typename Container::const_iterator it = values.begin();
    while (it != values.end()) {
        const int first = *it;
        ++it;
        if (it == values.end()) {
            // ペアにならなかった値は、ペアの処理後に挿入する
            Pair odd;
            odd.small = first;
            odd.large = first;
            pairs.push_back(odd);
            break;
        }

        const int second = *it;
        ++it;
        Pair current;
        if (first <= second) {
            current.small = first;
            current.large = second;
        } else {
            current.small = second;
            current.large = first;
        }
        pairs.push_back(current);
    }

    const bool hasOdd = values.size() % 2 != 0;
    const std::size_t pairCount = hasOdd ? pairs.size() - 1 : pairs.size();

    Container mainChain;
    for (std::size_t index = 0; index < pairCount; ++index)
        mainChain.push_back(pairs[index].large);
    fordJohnson(mainChain);

    // ソートされた大きい値を元のペアに対応させる
    // usedによって入力に重複がある場合も同じペアを二度使わない
    std::vector<Pair> ordered;
    std::vector<bool> used(pairCount, false);
    for (typename Container::const_iterator maxIt = mainChain.begin();
         maxIt != mainChain.end(); ++maxIt) {
        for (std::size_t index = 0; index < pairCount; ++index) {
            if (!used[index] && pairs[index].large == *maxIt) {
                ordered.push_back(pairs[index]);
                used[index] = true;
                break;
            }
        }
    }

    if (!ordered.empty()) {
        mainChain.insert(mainChain.begin(), ordered[0].small);
        const std::vector<std::size_t> order = insertionOrder(ordered.size());
        for (std::size_t index = 1; index < order.size(); ++index) {
            const Pair& pending = ordered[order[index]];
            insertBeforeAnchor(mainChain, pending.small, pending.large);
        }
    }

    if (hasOdd) {
        const int odd = pairs.back().small;
        typename Container::iterator position =
            lowerBound(mainChain.begin(), mainChain.end(), odd);
        mainChain.insert(position, odd);
    }
    values.swap(mainChain);
}

bool parsePositiveInt(const char *text, int& value) {
    if (text == 0 || *text == '\0')
        return false;
    char *end = 0;
    const long parsed = std::strtol(text, &end, 10);
    if (*end != '\0' || parsed <= 0 || parsed > INT_MAX)
        return false;
    value = static_cast<int>(parsed);
    return true;
}

}

// -----------------------------------------------------------------------------
// PmergeMeのメンバ関数
// -----------------------------------------------------------------------------

std::vector<int> PmergeMe::parseInput(int argc, char **argv) {
    if (argc < 2)
        throw std::runtime_error("at least one positive integer is required");

    std::vector<int> values;
    for (int index = 1; index < argc; ++index) {
        int value;
        if (!parsePositiveInt(argv[index], value))
            throw std::runtime_error(
                std::string("invalid positive integer: ") + argv[index]);
        values.push_back(value);
    }
    return values;
}

void PmergeMe::sortVector(std::vector<int>& values) {
    fordJohnson(values);
}

void PmergeMe::sortDeque(std::deque<int>& values) {
    fordJohnson(values);
}

void PmergeMe::print(const std::string& label,
                     const std::vector<int>& values) {
    std::cout << label;
    for (std::vector<int>::const_iterator it = values.begin();
         it != values.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << std::endl;
}
