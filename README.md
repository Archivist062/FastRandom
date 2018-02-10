# FastRandom

> Generate garbage, but do it with style

FastRandom is a small library made to generate random enough numbers and UUIDs. The randomity is not that guarranted, but at least, the output have really 100% of chance of being trash.

```c++
#include <FastRandom/UUID.h>

archivist::UUID weak{fast_uuid()}; // generate a hasty UUID
archivist::UUID medium{balanced_uuid()}; // generate a probably strong UUID
archivist::UUID good{strong_uuid()}; // generate a really random UUID
```

```c++
#include <FastRandom/Base_PRNG.h>

uint64_t number1 = archivist::PRNG(); // Generate a random number
uint64_t number2 = archivist::PRNG(4682); // Generate a random number and mixes some entropy (thread wide)
archivist::PRNG_feed(some_entropy); // Give some entropy system wide
```
