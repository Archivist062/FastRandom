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
		#ifdef COMPAT_TLS
			#ifdef NO_TLS
				extern uint64_t prngState;
				extern uint64_t prngState_c;
				extern uint64_t prngState_a;
				extern uint8_t balance;
			#else
				extern std::atomic<uint64_t> prngState;
				extern std::atomic<uint64_t> prngState_c;
				extern std::atomic<uint64_t> prngState_a;
				extern std::atomic<uint8_t> balance;
			#endif
		#else
			extern thread_local uint64_t prngState;
			extern thread_local uint64_t prngState_c;
			extern thread_local uint64_t prngState_a;
			extern thread_local uint8_t balance;
		#endif // COMPAT_TLS
		extern std::atomic<uint64_t> _entropy_contributor;
		uint64_t rnd();
	}

	inline uint32_t prng(uint64_t gift=42)
	{
		// Acquire potential entropy changes
		internal::prngState_c+= internal::_entropy_contributor.load(std::memory_order_relaxed);

		// LCG + gift
		internal::prngState_a= gift + (internal::prngState_a * 18446744073709551557ull)+internal::prngState_c;

		// +XOR LCG with the other state as mixer
		internal::prngState+=(internal::prngState^internal::prngState_a)+internal::prngState_c;

		return internal::prngState >> 32ull;
	}

	inline uint32_t prng_auto_feed()
	{
		internal::prngState_c+= internal::_entropy_contributor.load(std::memory_order_relaxed);
		internal::prngState_a= (internal::prngState_a * 18446744073709551557ull)+internal::prngState_c;

		if(internal::balance%32==0) {
			internal::prngState_a+=internal::prngState+internal::rnd();
		}

		internal::balance++;
		internal::prngState=(internal::prngState^internal::prngState_a)+internal::prngState_c;
		return internal::prngState >> 32ull;
	}

	inline void prng_feed(uint64_t feed)
	{
		internal::_entropy_contributor.fetch_xor(feed >> 32ull,std::memory_order_relaxed);
		prng(feed);
	}

	inline void prng_feed_alt(uint64_t feed)
	{
		internal::_entropy_contributor.fetch_add(feed >> 32ull,std::memory_order_relaxed);
		prng(feed);
	}
}