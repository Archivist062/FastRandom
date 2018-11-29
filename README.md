# FastRandom

> Generate garbage, but do it with style

FastRandom is a small library made to generate random enough numbers and UUIDs. The randomity is not that guarranted, but at least, the output have really 100% of chance of being trash.

For each batch of 64 bits generated, 54 are random AND independent (it is not me that says it, it is the code). This means the entropy you can expect of 1 UUID is at least 108 bits out of 128. This is sufficient for most purposes.

If you use the auto_fed PRNG, the guarranted entropy is of at least 56 bits, meaning a UUID has then 112 bits of entropy in balanced mode.

The point being that the generation of random data is faster than most other pseudo-random number generators.

```c++
#include <FastRandom/uuid.h>

archivist::UUID weak{fast_uuid()}; // generate a hasty UUID
archivist::UUID medium{balanced_uuid()}; // generate a probably strong UUID
archivist::UUID good{strong_uuid()}; // generate a really random UUID
```

```c++
#include <FastRandom/base_prng.h>

uint64_t number1 = archivist::prng(); // Generate a random number
uint64_t number2 = archivist::prng(4682); // Generate a random number and mixes some entropy (thread wide)
archivist::prng_feed(some_entropy); // Give some entropy system wide
```
