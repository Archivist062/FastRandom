/**
 * Licenced under MIT Licence
 *
 * (c) Ludovic 'Archivist' Lagouardette 2018
 *
 */

#pragma once
#include "FastRandom/base_prng.h"
#include <chrono>
#include <string>
#include <random>
#include <assert.h>

namespace archivist {

	class basic_uuid {
	public:
		uint64_t state[2];
		bool operator==(const basic_uuid& rhs) const
		{
			return (state[0]==rhs.state[0])&&(state[1]==rhs.state[1]);
		}
		bool operator<(const basic_uuid& rhs) const
		{
			return (state[0]<rhs.state[0])||((state[1]<rhs.state[1])&&(state[0]==rhs.state[0]));
		}
	};

	class fast_uuid : public basic_uuid {
	public:
		fast_uuid()
		{
			state[0]=prng(9998);
			state[1]=prng(17);
		}
	};

	class balanced_uuid : public basic_uuid {
	public:
		balanced_uuid()
		{
			state[0]=prng_auto_feed();
			state[1]=prng_auto_feed();
		}
	};

	class strong_uuid : public basic_uuid {
		static thread_local std::random_device src;
	public:
		strong_uuid()
		{
			uint32_t internal=src();
			uint32_t cross=src() + std::chrono::duration_cast< std::chrono::microseconds >(
			                   std::chrono::system_clock::now().time_since_epoch()
			               ).count();

			state[0]=prng(internal);
			prng_feed(cross);
			state[1]=prng(cross);
		}
	};

	static_assert(sizeof(basic_uuid)==16	,"basic_uuid not emplace constructible");
	static_assert(sizeof(fast_uuid)==16	,"fast_uuid not emplace constructible");
	static_assert(sizeof(balanced_uuid)==16,"balanced_uuid not emplace constructible");
	static_assert(sizeof(strong_uuid)==16	,"strong_uuid not emplace constructible");
}