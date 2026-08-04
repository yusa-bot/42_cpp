#ifndef WHATEVER_HPP
# define WHATEVER_HPP

// Key Point: 関数テンプレート

template <typename T>
void swap(T& first, T& second) {
    T temporary(first);

    first = second;
    second = temporary;
}

template <typename T>
const T& min(const T& first, const T& second) {
    return (first < second ? first : second);
}

template <typename T>
const T& max(const T& first, const T& second) {
    return (first > second ? first : second);
}

#endif

//普通の関数: .cppでコンパイルしておけば呼び出せる
//template: 呼び出す場所(main.cpp)のコンパイル時にどの型の関数を作るかを知る必要(実体化では、型ごとの専用関数を作る) -> .hppに実装する
