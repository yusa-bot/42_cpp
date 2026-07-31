
#ifndef SERIALIZER_HPP
# define SERIALIZER_HPP

# include <stdint.h>

# include "Data.hpp"

// ex目的: reinterpret_cast

class Serializer {
private:
    Serializer();
    Serializer(const Serializer& src);
    Serializer& operator=(const Serializer& rhs);
    ~Serializer();

public:
    static uintptr_t serialize(Data* ptr);
    static Data* deserialize(uintptr_t raw);
};

#endif

// static_cast: 通常の数値変換など
// reinterpret_cast: 別の型として再解釈する(アドレス値の変換)
// dynamic_cast: 継承クラスで実行時に安全確認しながら親子型を変換(実行時に確定しない)

// uintptr_t: アドレスを入れられる符号なし整数型
