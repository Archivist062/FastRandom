/**
 * Licenced under MIT Licence
 *  
 * (c) Ludovic 'Archivist' Lagouardette 2018
 *
 */

 #include "FastRandom/Base_PRNG.h"
#include <random>
namespace archivist{
thread_local uint64_t internal::prngState=0x7A7A7A7A5B5B5B5B;
thread_local uint64_t internal::prngState_a=0x00;
thread_local uint64_t internal::prngState_c=0x7A7A6565655B5B;
std::atomic<uint64_t> internal::_entropy_contributor;

class dummy168783486732{
public:
	dummy168783486732()
	{
		std::random_device rnd;
		internal::prngState_a=rnd()*0x0000000100000000+rnd();
		PRNG_feed(rnd());
	}
};
thread_local dummy168783486732 dummy168783486732_inst;
}