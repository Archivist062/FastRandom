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
		#ifndef COMPAT_TLS
			extern thread_local uint64_t prngState;
			extern thread_local uint64_t prngState_c;
			extern thread_local uint64_t prngState_a;
			extern thread_local uint8_t balance;
		#else
			extern std::atomic<uint64_t> prngState;
			extern std::atomic<uint64_t> prngState_c;
			extern std::atomic<uint64_t> prngState_a;
			extern std::atomic<uint8_t> balance;
		#endif // COMPAT_TLS
		extern std::atomic<uint64_t> _entropy_contributor;
		uint64_t rnd();
	}

	inline uint64_t prng(uint64_t gift=42)
	{
		internal::prngState_c+= internal::_entropy_contributor.load(std::memory_order_relaxed);
		internal::prngState_a= (internal::prngState_a << 17)+internal::prngState_c;
		internal::prngState+=gift;
		internal::prngState=internal::prngState*internal::prngState_a+internal::prngState_c;
		return internal::prngState;
	}

	inline uint64_t prng_auto_feed()
	{
		internal::prngState_c+= internal::_entropy_contributor.load(std::memory_order_relaxed);
		internal::prngState_a= (internal::prngState_a << 17)+internal::prngState_c;

		if(internal::balance%32==0) {
			internal::prngState=(internal::prngState<<1)+internal::rnd();
		}

		internal::balance++;
		internal::prngState=internal::prngState*internal::prngState_a+internal::prngState_c;
		return internal::prngState;
	}

	inline void prng_feed(uint64_t feed)
	{
		internal::_entropy_contributor.fetch_xor(feed,std::memory_order_relaxed);
	}

	inline void prng_feed_alt(uint64_t feed)
	{
		internal::_entropy_contributor.fetch_add(feed,std::memory_order_relaxed);
	}
}