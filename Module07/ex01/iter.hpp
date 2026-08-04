#ifndef ITER_HPP
# define ITER_HPP

# include <cstddef>

// Key Point: 関数テンプレート + 関数を引数に取る
// const 要素には const 参照を受け取る関数だけ適用できる

template <typename T, typename F>
void iter(T *array, const std::size_t length, F function) {
    for (std::size_t index = 0; index < length; ++index)
        function(array[index]);
}

#endif
