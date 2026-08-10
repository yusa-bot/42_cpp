#include "PmergeMe.hpp"

#include <climits>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

// -----------------------------------------------------------------------------
// namespace
// -----------------------------------------------------------------------------

namespace {

struct Element {
    int value;
    std::size_t id;
};

struct Pair {
    Element small;
    Element large;
};

struct Pending {
    Element value; // Pairのsmall
    std::size_t anchorId; // PairのlargeのID
    bool hasAnchor; // Pairのlargeの有無
};

Pair makePair(const Element& first, const Element& second) {
    Pair pair;

    if (first.value <= second.value) {
        pair.small = first;
        pair.large = second;
    } else {
        pair.small = second;
        pair.large = first;
    }
    return pair;
}

int parsePositiveInteger(const char *text) {
    if (text == 0 || *text == '\0')
        throw std::runtime_error("invalid positive integer");

    int value = 0;
    for (std::size_t index = 0; text[index] != '\0'; ++index) {
        if (text[index] < '0' || text[index] > '9')
            throw std::runtime_error(std::string("invalid positive integer: ") + text);
        const int digit = text[index] - '0';
        if (value > (INT_MAX - digit) / 10)
            throw std::runtime_error(std::string("integer is out of range: ") + text);
        value = value * 10 + digit;
    }
    if (value == 0)
        throw std::runtime_error(std::string("invalid positive integer: ") + text);
    return value;
}

void readBefore(int argc, char **argv, std::vector<int>& values) {
    if (argc < 2)
        throw std::runtime_error("at least one positive integer is required");

    values.reserve(static_cast<std::size_t>(argc - 1)); // vectorを数字個数分 確保
    for (int index = 1; index < argc; ++index)
        values.push_back(parsePositiveInteger(argv[index]));
}

double elapsedMicroseconds(std::clock_t start, std::clock_t finish) {
    return 1000000.0 * static_cast<double>(finish - start)
        / static_cast<double>(CLOCKS_PER_SEC);
}

// --------------------------------------------------------------------------
// container1: std::vector
// --------------------------------------------------------------------------

typedef std::vector<Element> VectorElements;
typedef std::vector<Pair> VectorPairs;
typedef std::vector<Pending> VectorPending;
typedef std::vector<std::size_t> VectorOrder;

void fillVector(int argc, char **argv, VectorElements& values) {
    values.reserve(static_cast<std::size_t>(argc - 1));

    for (int index = 1; index < argc; ++index) {
        Element element;
        element.value = parsePositiveInteger(argv[index]);
        element.id = static_cast<std::size_t>(index - 1);
        values.push_back(element);
    }
}

// --------------- fordJohnsonVector ---------------

VectorOrder vectorInsertionOrder(std::size_t count) {
    VectorOrder order; // pending の添字を保存するvector
    if (count == 0)
        return order;

    order.reserve(count);
    order.push_back(0);
    std::size_t previous = 1;
    std::size_t current = 3;
    while (previous < count) {
        const std::size_t end = current < count ? current : count;
        for (std::size_t index = end; index > previous; --index)
            order.push_back(index - 1);
        const std::size_t next = current + 2 * previous;
        previous = current;
        current = next;
    }
    return order;
}

std::size_t vectorAnchorIndex(const VectorElements& chain,
                              std::size_t anchorId) {
    for (std::size_t index = 0; index < chain.size(); ++index) {
        if (chain[index].id == anchorId)
            return index;
    }
    throw std::logic_error("Ford-Johnson anchor was not found");
}

std::size_t vectorLowerBound(const VectorElements& chain,
                             std::size_t end, int value) {
    std::size_t first = 0;
    while (first < end) {
        const std::size_t middle = first + (end - first) / 2;
        if (chain[middle].value < value)
            first = middle + 1;
        else
            end = middle;
    }
    return first;
}


// ex.)  0:3 1:5 2:9 3:7 4:4
void fordJohnsonVector(VectorElements& values, std::size_t totalCount) {
    // 再帰の出口
    if (values.size() < 2)
        return;

    // 奇数個の場合unpairedを取得 -> unpairedElement
    const bool hasUnpairedElement = values.size() % 2 != 0;
    Element unpairedElement;
    if (hasUnpairedElement)
        unpairedElement = values.back(); // ex.) 4:4


    // ----------- 再帰内処理 -----------
    // 数字を2つづつのペアに -> pairs ex.) S0:3/L1:5, S3:7/L2:9
    VectorPairs pairs;
    pairs.reserve(values.size() / 2);
    for (std::size_t index = 0; index + 1 < values.size(); index += 2)
        pairs.push_back(makePair(values[index], values[index + 1]));

    // 大きい要素を再帰的にソートする。IDによってペアの対応関係を維持する
    VectorElements mainChain;
    mainChain.reserve(values.size());

    // pairForLargeIdの目的: 再帰ソートで順番が変わってもペア関係を復元できる, IDからペア番号を O(1) で取得できる
    std::vector<std::size_t> pairForLargeId(totalCount, totalCount);
    // ↑要素数と初期値を指定するコンストラクタ ex.) [5, 5, 5, 5, 5]

    // largeを取り出す
    for (std::size_t index = 0; index < pairs.size(); ++index) {
        mainChain.push_back(pairs[index].large); // ex.) 1:5, 2:9
        // pairのlargeのidの箇所に、pairsのidを記録 -> pairsを記録でき、
        pairForLargeId[pairs[index].large.id] = index; // ex.) 1に0, 2に1
    }

    // pairsのlargeのみで再帰
    fordJohnsonVector(mainChain, totalCount);

    // -> 今度は呼び出し元へ戻りながらsmallを挿入していく



    // ソートされた大きい要素の順番に合わせて、小さい要素を並べる
    // ペアにならなかった要素は、探索範囲に上限がない最後の挿入候補にする



    // ------ pending作成 ------
    // pending: 今のmainChainに挿入するsmallの数字の並びを保持
    VectorPending pending;
    pending.reserve(pairs.size() + (hasUnpairedElement ? 1 : 0));

    // ソート済みのlargeに対応するsmallを並べる
    for (std::size_t index = 0; index < mainChain.size(); ++index) {

        // Pair ID を取得
         const std::size_t pairIndex = pairForLargeId[mainChain[index].id];
        if (pairIndex == totalCount)
            throw std::logic_error("Ford-Johnson pair was not found");

        Pending item;
        item.value = pairs[pairIndex].small;
        item.anchorId = pairs[pairIndex].large.id;
        item.hasAnchor = true;
        // -> Pairのlargeより前にsmallを挿入可能に

        pending.push_back(item);
    }

    if (hasUnpairedElement) {
        Pending item;
        item.value = unpairedElement;
        item.anchorId = 0; // dummy
        item.hasAnchor = false;
        pending.push_back(item);
    }
    // ------ pending完成 ------


    // 完成した pending をJacobsthal順で取り出し、mainChain の適切な位置へ挿入する処理
    // ex.) mainChain: 1:5, 2:9 / pending: 0:3, 1:7, 2:4

    const VectorOrder order = vectorInsertionOrder(pending.size());

    for (std::size_t index = 0; index < order.size(); ++index) {
        const Pending& item = pending[order[index]];

        const std::size_t end = item.hasAnchor
            ? vectorAnchorIndex(mainChain, item.anchorId)
            : mainChain.size();

        const std::size_t position =
            vectorLowerBound(mainChain, end, item.value.value);
        mainChain.insert(mainChain.begin() + position, item.value);
    }

    // result return
    values.swap(mainChain);
}

void copyVectorResult(const VectorElements& source,
                      std::vector<int>& destination) {
    destination.reserve(source.size());
    for (VectorElements::const_iterator it = source.begin();
         it != source.end(); ++it)
        destination.push_back(it->value);
}

// --------------------------------------------------------------------------
// container2: std::deque
// --------------------------------------------------------------------------

typedef std::deque<Element> DequeElements;
typedef std::deque<Pair> DequePairs;
typedef std::deque<Pending> DequePending;
typedef std::deque<std::size_t> DequeOrder;

void fillDeque(int argc, char **argv, DequeElements& values) {
    for (int index = 1; index < argc; ++index) {
        Element element;
        element.value = parsePositiveInteger(argv[index]);
        element.id = static_cast<std::size_t>(index - 1);
        values.push_back(element);
    }
}

std::size_t dequeLowerBound(const DequeElements& chain,
                            std::size_t end, int value) {
    std::size_t first = 0;
    while (first < end) {
        const std::size_t middle = first + (end - first) / 2;
        if (chain[middle].value < value)
            first = middle + 1;
        else
            end = middle;
    }
    return first;
}

std::size_t dequeAnchorIndex(const DequeElements& chain,
                             std::size_t anchorId) {
    for (std::size_t index = 0; index < chain.size(); ++index) {
        if (chain[index].id == anchorId)
            return index;
    }
    throw std::logic_error("Ford-Johnson anchor was not found");
}

DequeOrder dequeInsertionOrder(std::size_t count) {
    DequeOrder order;
    if (count == 0)
        return order;

    order.push_back(0);
    std::size_t previous = 1;
    std::size_t current = 3;
    while (previous < count) {
        const std::size_t end = current < count ? current : count;
        for (std::size_t index = end; index > previous; --index)
            order.push_back(index - 1);
        const std::size_t next = current + 2 * previous;
        previous = current;
        current = next;
    }
    return order;
}

void fordJohnsonDeque(DequeElements& values, std::size_t totalCount) {
    if (values.size() < 2)
        return;

    // 共通のテンプレート関数は使わず、vector版と同じ処理をdeque用に実装する
    const bool hasUnpairedElement = values.size() % 2 != 0;
    Element unpairedElement;
    if (hasUnpairedElement)
        unpairedElement = values.back();

    DequePairs pairs;
    for (std::size_t index = 0; index + 1 < values.size(); index += 2)
        pairs.push_back(makePair(values[index], values[index + 1]));

    DequeElements mainChain;
    std::deque<std::size_t> pairForLargeId(totalCount, totalCount);
    for (std::size_t index = 0; index < pairs.size(); ++index) {
        mainChain.push_back(pairs[index].large);
        pairForLargeId[pairs[index].large.id] = index;
    }
    fordJohnsonDeque(mainChain, totalCount);

    DequePending pending;
    for (std::size_t index = 0; index < mainChain.size(); ++index) {
        const std::size_t pairIndex = pairForLargeId[mainChain[index].id];
        if (pairIndex == totalCount)
            throw std::logic_error("Ford-Johnson pair was not found");
        Pending item;
        item.value = pairs[pairIndex].small;
        item.anchorId = pairs[pairIndex].large.id;
        item.hasAnchor = true;
        pending.push_back(item);
    }
    if (hasUnpairedElement) {
        Pending item;
        item.value = unpairedElement;
        item.anchorId = 0;
        item.hasAnchor = false;
        pending.push_back(item);
    }

    const DequeOrder order = dequeInsertionOrder(pending.size());
    for (std::size_t index = 0; index < order.size(); ++index) {
        const Pending& item = pending[order[index]];
        const std::size_t end = item.hasAnchor
            ? dequeAnchorIndex(mainChain, item.anchorId)
            : mainChain.size();
        const std::size_t position =
            dequeLowerBound(mainChain, end, item.value.value);
        mainChain.insert(mainChain.begin() + position, item.value);
    }
    values.swap(mainChain);
}

void copyDequeResult(const DequeElements& source,
                     std::deque<int>& destination) {
    for (DequeElements::const_iterator it = source.begin();
         it != source.end(); ++it)
        destination.push_back(it->value);
}

void printVector(const std::string& label,
                 const std::vector<int>& values) {
    std::cout << label;
    for (std::vector<int>::const_iterator it = values.begin();
         it != values.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << std::endl;
}

}

// --------------------------------------------------------------------------
// OCF
// --------------------------------------------------------------------------

PmergeMe::PmergeMe()
    : _before(),
      _vectorResult(),
      _dequeResult(),
      _vectorTime(0.0),
      _dequeTime(0.0) {
} // コンストラクタがない組み込み型のみ引数で初期化

PmergeMe::PmergeMe(const PmergeMe& other)
    : _before(other._before),
      _vectorResult(other._vectorResult),
      _dequeResult(other._dequeResult),
      _vectorTime(other._vectorTime),
      _dequeTime(other._dequeTime) {
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        PmergeMe copy(other);
        swap(copy);
    }
    return *this;
}

// 2つの PmergeMe オブジェクトの中身を交換する(thisとother)
void PmergeMe::swap(PmergeMe& other) {
    // vector, dequeにはメンバ関数 swap() がある
    _before.swap(other._before);
    _vectorResult.swap(other._vectorResult);
    _dequeResult.swap(other._dequeResult);

    // doubleは手動swap
    const double vectorTime = _vectorTime;
    _vectorTime = other._vectorTime;
    other._vectorTime = vectorTime;

    const double dequeTime = _dequeTime;
    _dequeTime = other._dequeTime;
    other._dequeTime = dequeTime;
}

PmergeMe::~PmergeMe() {
}

// --------------------------------------------------------------------------
// Public interface
// --------------------------------------------------------------------------

void PmergeMe::process(int argc, char **argv) {
    // ------ parse ------
    std::vector<int> before;
    readBefore(argc, argv, before); // ここまでどちらもvector　(課題指示の「コンテナ別にデータ管理も計測する」ため、beforeは再利用しない)

    // ------ container1: vector ------
    const std::clock_t vectorStart = std::clock();

    VectorElements vectorValues;
    std::vector<int> vectorResult;

    fillVector(argc, argv, vectorValues);
    fordJohnsonVector(vectorValues, vectorValues.size());
    copyVectorResult(vectorValues, vectorResult);

    const std::clock_t vectorFinish = std::clock();

    // ------ container2: deque ------
    const std::clock_t dequeStart = std::clock();

    DequeElements dequeValues;
    std::deque<int> dequeResult;
    fillDeque(argc, argv, dequeValues);
    fordJohnsonDeque(dequeValues, dequeValues.size());
    copyDequeResult(dequeValues, dequeResult);

    const std::clock_t dequeFinish = std::clock();

    // ------ ------
    _before.swap(before);
    _vectorResult.swap(vectorResult);
    _dequeResult.swap(dequeResult);

    _vectorTime = elapsedMicroseconds(vectorStart, vectorFinish);
    _dequeTime = elapsedMicroseconds(dequeStart, dequeFinish);
}

void PmergeMe::display() const {
    printVector("Before:", _before);
    printVector("After:", _vectorResult);

    const std::ios::fmtflags previousFlags = std::cout.flags();
    const std::streamsize previousPrecision = std::cout.precision();
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << _vectorResult.size()
              << " elements with std::vector: "
              << _vectorTime << " us" << std::endl;
    std::cout << "Time to process a range of " << _dequeResult.size()
              << " elements with std::deque: "
              << _dequeTime << " us" << std::endl;
    std::cout.flags(previousFlags);
    std::cout.precision(previousPrecision);
}
