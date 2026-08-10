#ifndef ARRAY_HPP
# define ARRAY_HPP

# include <stdexcept>

// Key Point: クラステンプレート + 動的配列 + 深いコピー

template <typename T>
class Array {
private:
    T* _elements;
    unsigned int _size;

public:
    Array() : _elements(0), _size(0) {}

    Array(unsigned int n) : _elements(0), _size(n) {
        if (n > 0)
            _elements = new T[n]();
    }

    Array(const Array& other) : _elements(0), _size(0) {
        *this = other;
    }

    ~Array() {
        delete[] _elements;
    }

    Array& operator=(const Array& other) {
        T *newElements = 0;

        if (this == &other) // アドレス
            return *this;

        if (other._size > 0) {
            newElements = new T[other._size](); // int型等の値を初期化
            try {
                for (unsigned int index = 0; index < other._size; ++index)
                    newElements[index] = other._elements[index]; // ここではT型のoperator=
            } catch (...) { // どんな型のerrorでもdelete[]するため
                delete[] newElements;
                throw; // 元の例外の型・内容を保ったまま、operator= を呼んだ側の catch まで伝播
            }
        }

        delete[] _elements; // thisの元配列
        _elements = newElements;
        _size = other._size;
        return *this;
    }

    T& operator[](unsigned int index) {
        if (index >= _size)
            throw std::out_of_range("Array index is out of bounds");
        return _elements[index];
    }

    const T& operator[](unsigned int index) const {
        if (index >= _size)
            throw std::out_of_range("Array index is out of bounds");
        return _elements[index];
    }

    unsigned int size() const {
        return _size;
    }
};

#endif
