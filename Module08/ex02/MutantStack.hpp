#ifndef MUTANTSTACK_HPP
# define MUTANTSTACK_HPP

# include <deque>
# include <stack>

// Key Point: コンテナアダプタ(NOT iterable)

template <typename T, typename Container = std::deque<T> > // stackはrapperのためこの2つ必要
class MutantStack : public std::stack<T, Container> {

public:
    typedef typename std::stack<T, Container>::container_type::iterator // std::stackの内部のcontainer型のiteratorを取得
        iterator; // ↑をiteratorと名付ける
    typedef typename std::stack<T, Container>::container_type::const_iterator
        const_iterator;

    // ------- OCF -------
    MutantStack() : std::stack<T, Container>() {
    } // 基底クラスstd::stackのコンストラクタを先に呼ぶ

    MutantStack(const MutantStack& other)
        : std::stack<T, Container>(other) {
    }

    MutantStack& operator=(const MutantStack& other) {
        if (this != &other)
            std::stack<T, Container>::operator=(other);
        return *this;
    }

    ~MutantStack() {
    }

    // ------- func -------
    iterator begin() {
        return this->c.begin();
    } // c: 基底クラスstd::stackの 内部コンテナc

    iterator end() {
        return this->c.end();
    }

    const_iterator begin() const {
        return this->c.begin();
    }

    const_iterator end() const {
        return this->c.end();
    }
};

#endif
