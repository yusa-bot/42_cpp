#include "PmergeMe.hpp"

#include <algorithm>
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
    // 各再帰レベルでペアになったsmallを保持する。
    std::vector<Element *> smalls;

    explicit Element(int initialValue)
        : value(initialValue), smalls() {
    }
};

struct Pair {
    Element *small;
    Element *large;
};

struct Pending {
    Element *small;
    Element *pairedLarge;

    Pending(Element *pendingSmall, Element *pairedLargeElement)
        : small(pendingSmall), pairedLarge(pairedLargeElement) {
    }
};

// 大小を判定
Pair makePair(Element *first, Element *second) {
    Pair pair;

    if (first->value <= second->value) {
        pair.small = first;
        pair.large = second;
    } else {
        pair.small = second;
        pair.large = first;
    }
    return pair;
}

struct ElementPointerLess {
    bool operator()(const Element *left, const Element *right) const {
        return left->value < right->value;
    }
};

typedef std::vector<Pending> PendingItems;
typedef std::vector<std::size_t> InsertionOrder;

// Jacobsthal順を作る
// どの順番でmain chainへ挿入するかを添字で生成
// 境界(挿入グループの最後の番号)でsmallをグループ化し、各グループ内を後ろから挿入
InsertionOrder makeInsertionOrder(std::size_t count) { // 入力はPendingの個数

    // 次にどのpendingを選ぶか
    InsertionOrder order;
    if (count == 0)
        return order;

    order.reserve(count);
    // b1はソート済みa1の直前に比較なしで挿入できる。
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

// largeからその再帰階層のsmallを取り出し、Pendingを作成
Pending takePending(Element *large) {
    if (large->smalls.empty())
        throw std::logic_error("Ford-Johnson small element was not found");

    // 比較関係は再帰が深くなるにつれて末尾へ追加 -> 再帰から戻る順番はその逆なのでback
    Element *small = large->smalls.back();
    large->smalls.pop_back();
    return Pending(small, large);
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

typedef std::vector<Element> VectorNodes;
typedef std::vector<Element *> VectorElements;

// 各整数を Element にする
void fillVector(int argc, char **argv, VectorNodes& nodes,
                VectorElements& values) {
    const std::size_t count = static_cast<std::size_t>(argc - 1);
    nodes.reserve(count);
    values.reserve(count);

    for (int index = 1; index < argc; ++index)
        nodes.push_back(Element(parsePositiveInteger(argv[index])));
    for (VectorNodes::iterator it = nodes.begin(); it != nodes.end(); ++it)
        values.push_back(&*it);
}

void fordJohnsonVector(VectorElements& values) {
    if (values.size() < 2)
        return;

    const bool hasUnpairedElement = values.size() % 2 != 0;
    Element *unpairedElement = hasUnpairedElement ? values.back() : 0;

    VectorElements larges;
    larges.reserve(values.size() / 2);
    for (std::size_t index = 0; index + 1 < values.size(); index += 2) {
        const Pair pair = makePair(values[index], values[index + 1]);
        pair.large->smalls.push_back(pair.small);
        larges.push_back(pair.large);
    }

    // 再帰
    fordJohnsonVector(larges);
    VectorElements mainChain(larges);
    mainChain.reserve(values.size());

    PendingItems pending;
    pending.reserve(larges.size() + (hasUnpairedElement ? 1 : 0));
    for (VectorElements::iterator it = larges.begin();
         it != larges.end(); ++it)
        pending.push_back(takePending(*it));
    if (hasUnpairedElement)
        pending.push_back(Pending(unpairedElement, 0));

    const InsertionOrder order = makeInsertionOrder(pending.size());
    mainChain.insert(mainChain.begin(), pending[0].small);

    // 次のpendingを選ぶ
    for (std::size_t index = 1; index < order.size(); ++index) {
        const Pending& item = pending[order[index]];
        VectorElements::iterator bound = mainChain.end();
        if (item.pairedLarge != 0) {
            bound = std::find(mainChain.begin(), mainChain.end(),
                              item.pairedLarge);
            if (bound == mainChain.end())
                throw std::logic_error(
                    "Ford-Johnson paired large was not found");
        }

        // lower_bound: 指定値(ここでは挿入値)以上になる最初の位置を返す標準アルゴリズム
        const VectorElements::iterator position = std::lower_bound(
            mainChain.begin(), bound, item.small, ElementPointerLess());
        mainChain.insert(position, item.small);
    }
    values.swap(mainChain);
}

void copyVectorResult(const VectorElements& source,
                      std::vector<int>& destination) {
    destination.reserve(source.size());
    for (VectorElements::const_iterator it = source.begin();
         it != source.end(); ++it)
        destination.push_back((*it)->value);
}

// --------------------------------------------------------------------------
// container2: std::deque
// --------------------------------------------------------------------------

typedef std::deque<Element> DequeNodes;
typedef std::deque<Element *> DequeElements;

void fillDeque(int argc, char **argv, DequeNodes& nodes,
               DequeElements& values) {
    for (int index = 1; index < argc; ++index)
        nodes.push_back(Element(parsePositiveInteger(argv[index])));
    for (DequeNodes::iterator it = nodes.begin(); it != nodes.end(); ++it)
        values.push_back(&*it);
}

void fordJohnsonDeque(DequeElements& values) {
    if (values.size() < 2)
        return;

    const bool hasUnpairedElement = values.size() % 2 != 0;
    Element *unpairedElement = hasUnpairedElement ? values.back() : 0;

    DequeElements larges;
    for (std::size_t index = 0; index + 1 < values.size(); index += 2) {
        const Pair pair = makePair(values[index], values[index + 1]);
        pair.large->smalls.push_back(pair.small);
        larges.push_back(pair.large);
    }

    // 再帰
    fordJohnsonDeque(larges);
    DequeElements mainChain(larges);

    PendingItems pending;
    pending.reserve(larges.size() + (hasUnpairedElement ? 1 : 0));
    for (DequeElements::iterator it = larges.begin();
         it != larges.end(); ++it)
        pending.push_back(takePending(*it));
    if (hasUnpairedElement)
        pending.push_back(Pending(unpairedElement, 0));

    const InsertionOrder order = makeInsertionOrder(pending.size());
    mainChain.insert(mainChain.begin(), pending[0].small);

    // 次のpendingを選ぶ
    for (std::size_t index = 1; index < order.size(); ++index) {
        const Pending& item = pending[order[index]];
        DequeElements::iterator bound = mainChain.end();
        if (item.pairedLarge != 0) {
            bound = std::find(mainChain.begin(), mainChain.end(),
                              item.pairedLarge);
            if (bound == mainChain.end())
                throw std::logic_error(
                    "Ford-Johnson paired large was not found");
        }

        // main chainのどこへ挿入するか (pairedLargeより前のみ探索)
        const DequeElements::iterator position = std::lower_bound(
            mainChain.begin(), bound, item.small, ElementPointerLess());
        mainChain.insert(position, item.small);
    }
    values.swap(mainChain);
}

void copyDequeResult(const DequeElements& source,
                     std::deque<int>& destination) {
    for (DequeElements::const_iterator it = source.begin();
         it != source.end(); ++it)
        destination.push_back((*it)->value);
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

    VectorNodes vectorNodes;
    VectorElements vectorValues;
    std::vector<int> vectorResult;

    fillVector(argc, argv, vectorNodes, vectorValues);
    fordJohnsonVector(vectorValues);
    copyVectorResult(vectorValues, vectorResult);

    const std::clock_t vectorFinish = std::clock();

    // ------ container2: deque ------
    const std::clock_t dequeStart = std::clock();

    DequeNodes dequeNodes;
    DequeElements dequeValues;
    std::deque<int> dequeResult;
    fillDeque(argc, argv, dequeNodes, dequeValues);
    fordJohnsonDeque(dequeValues);
    copyDequeResult(dequeValues, dequeResult);

    const std::clock_t dequeFinish = std::clock();

    if (vectorResult.size() != dequeResult.size()
        || !std::equal(vectorResult.begin(), vectorResult.end(),
                       dequeResult.begin()))
        throw std::logic_error("container results do not match");

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

// 「何番目のpendingを挿入するか」は共通化し、「そのコンテナへどう挿入するか」は個別実装

//共通
//├── Element / Pair / Pending
//├── smallとlargeの対応管理
//├── makePair()
//├── Jacobsthal挿入順
//└── Pending用vector

//コンテナ固有
//├── 要素・large・main chain
//├── 再帰処理
//├── pairedLarge探索
//├── lower_bound
//└── main chainへの挿入
