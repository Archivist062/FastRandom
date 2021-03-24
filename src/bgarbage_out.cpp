#include "FastRandom/base_prng.h"

#include <iostream>
#include <array>
#include <string_view>

int main() {
    std::ios_base::sync_with_stdio(false);
    while(true) {
        union {
            uint32_t v = archivist::prng_auto_feed();
            std::array<char, 4> ary;
        };
        std::cout << std::string_view(ary.data(), 4);
    }
}