/**
 * Licenced under MIT Licence
 *
 * (c) Ludovic 'Archivist' Lagouardette 2018
 *
 */

#pragma once
#include <mutex>
#include <stdint.h>
#include <atomic>

namespace archivist {

	namespace internal {
		extern thread_local uint64_t prngState;
		extern thread_local uint64_t prngState_c;
		extern thread_local uint64_t prngState_a;
		extern std::atomic<uint64_t> _entropy_contributor;
	}

	inline uint64_t PRNG(uint64_t gift=42)
	{
		internal::prngState_c+= internal::_entropy_contributor.load(std::memory_order_relaxed);
		internal::prngState_a= (internal::prngState_a << 17)+internal::prngState_c;
		internal::prngState^=gift;
		internal::prngState=internal::prngState*internal::prngState_a+internal::prngState_c;
		return internal::prngState;
	}

	inline void PRNG_feed(uint64_t feed)
	{
		internal::_entropy_contributor.fetch_xor(feed,std::memory_order_relaxed);
	}

	inline void PRNG_feed_alt(uint64_t feed)
	{
		internal::_entropy_contributor.fetch_add(feed,std::memory_order_relaxed);
	}
}