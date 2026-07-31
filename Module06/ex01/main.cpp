#include <iostream>

#include "Data.hpp"
#include "Serializer.hpp"

int main() {
    Data original;
    original.id = 42;
    original.name = "Serializer test";

    uintptr_t raw = Serializer::serialize(&original);
    Data* restored = Serializer::deserialize(raw);

    std::cout << "original: " << &original << std::endl;
    std::cout << "serialized: " << raw << std::endl;
    std::cout << "deserialized: " << restored << std::endl;
    std::cout << "same pointer: "
              << (&original == restored ? "true" : "false") << std::endl;
    std::cout << "data: " << restored->id << ", " << restored->name << std::endl;
    return (&original == restored ? 0 : 1);
}
