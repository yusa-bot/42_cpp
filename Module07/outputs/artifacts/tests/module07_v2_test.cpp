#include "../../../ex02/Array.hpp"
#include "../../../ex01/iter.hpp"
#include "../../../ex00/whatever.hpp"

#include <cassert>
#include <climits>
#include <iostream>
#include <stdexcept>

struct Tracked {
    static int live;
    static int assignments;
    int value;
    Tracked() : value(0) { ++live; }
    Tracked(const Tracked& other) : value(other.value) { ++live; }
    ~Tracked() { --live; }
    Tracked& operator=(const Tracked& other) {
        if (++assignments == 2)
            throw std::runtime_error("copy assignment failure");
        value = other.value;
        return *this;
    }
};
int Tracked::live = 0;
int Tracked::assignments = 0;

static void accepts_array(Array<int>) {}

int main() {
    Array<int> values(2);
    values[0] = 17;

    volatile unsigned long runtime_uint_max = UINT_MAX;
    const unsigned long beyond_uint = runtime_uint_max + 1UL;
    bool threw = false;
    try { (void)values[beyond_uint]; }
    catch (const std::exception&) { threw = true; }
    std::cout << "wide index throws: "
              << (threw ? "yes" : "NO (truncated)") << '\n';

    accepts_array(2u); // Demonstrates the converting constructor.

    Array<Tracked> source(3);
    Array<Tracked> destination(1);
    const int live_before = Tracked::live;
    bool copy_threw = false;
    try { destination = source; }
    catch (const std::runtime_error&) { copy_threw = true; }
    assert(copy_threw);
    assert(destination.size() == 1);
    std::cout << "live after throwing assignment: " << Tracked::live
              << " (before " << live_before << ")\n";
    assert(Tracked::live == live_before);
}
