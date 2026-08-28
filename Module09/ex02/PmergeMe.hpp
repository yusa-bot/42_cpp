#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <deque>
# include <vector>

// Key Point:
// containerごとにalgorithm, 同一処理は同じに。

//共通化する
//├── 入力解析
//├── 時間変換
//├── Element / Pair / Pending
//├── 2要素をsmall/largeに分ける処理
//└── Jacobsthal境界の純粋な数値計算

//コンテナごとに実装する
//├── Ford–Johnson再帰本体
//├── main chainの構築
//├── pendingの構築
//├── anchor検索
//├── 二分探索
//├── 挿入順を保存するコンテナ
//└── main chainへの挿入

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
