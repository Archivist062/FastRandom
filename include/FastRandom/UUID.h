/**
 * Licenced under MIT Licence
 *  
 * (c) Ludovic 'Archivist' Lagouardette 2018
 *
 */

#pragma once
#include "FastRandom/Base_PRNG.h"
#include <chrono>
#include <string>
#include <random>

namespace archivist{

	class fast_uuid{};
	class balanced_uuid{};
	class strong_uuid{};

	struct UUID{
		static thread_local std::random_device src;
		static thread_local uint8_t balance;
		uint64_t state[2];
		UUID(fast_uuid) : UUID(){}
		UUID()
		{
			state[0]=PRNG(9998);
			state[1]=PRNG(17);
		}
		UUID(strong_uuid)
		{
			uint32_t internal=src();
			uint32_t cross=src() | std::chrono::duration_cast< std::chrono::microseconds >(
				std::chrono::system_clock::now().time_since_epoch()
			).count();

			state[0]=PRNG(internal);
			PRNG_feed(cross);
			state[1]=PRNG(cross);
		}
		UUID(balanced_uuid)
		{
			balance++;
			if(balance!=0)
			{
				*this=UUID();
			}
			else
			{
				*this=UUID(strong_uuid());
			}
		}
		bool operator==(const UUID& rhs) const
		{
			return (state[0]==rhs.state[0])&&(state[1]==rhs.state[1]);
		}
		bool operator<(const UUID& rhs) const
		{
			return (state[0]<rhs.state[0])||((state[1]<rhs.state[1])&&(state[0]==rhs.state[0]));
		}
	};

}