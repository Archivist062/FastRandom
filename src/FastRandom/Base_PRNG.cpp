/**
 * Licenced under MIT Licence
 *  
 * (c) Ludovic 'Archivist' Lagouardette 2018
 *
 */

#include "FastRandom/Base_PRNG.h"
#include <random>
#include <set>
#include <iostream>
#include "catch.hpp"

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

TEST_CASE("Testing the PRNG basic generator"){
	const size_t loop = 5000000;
	std::set<uint64_t> rec;
	for(size_t i=0;i<loop;i++)
	{
		uint64_t data=archivist::PRNG();
		REQUIRE(rec.find(data)==rec.end());
		rec.insert(data);
	}
}

TEST_CASE("Testing the PRNG basic generator (statistics)"){
	const size_t loop = 500000;
	std::set<uint64_t> rec;
	for(size_t i=0;i<loop;i++)
	{
		uint64_t data=archivist::PRNG();
		rec.insert(data);
	}
	int cumul[64]={};
	for(auto val : rec)
	{
		for(auto b=0;b<64;b++)
		{
			cumul[b]+= ((1<<b)&val)!=0;
		}
	}
	float stats[64]={};
	int ok_bits=0;
	for(auto b=0;b<64;b++)
	{
		stats[b]= cumul[b]/(float)loop;
		ok_bits+=(stats[b]==Approx(0.5).epsilon(0.08));
	}
	REQUIRE(ok_bits>=56);
}

TEST_CASE("Testing the PRNG basic generator (statistics 2)"){
	const size_t loop = 500000;
	std::set<uint64_t> rec;
	for(size_t i=0;i<loop;i++)
	{
		uint64_t data=archivist::PRNG();
		rec.insert(data);
	}
	int cumul[64][64]={};
	for(auto val : rec)
	{
		for(auto b=0;b<64;b++)
		{
			for(auto c=0;c<64;c++)
			{
				cumul[b][c]+= (((1<<b)&val)!=0) && (((1<<c)&val)!=0);
			}
		}
	}
	float stats[64][64]={};
	int ok_bits=0;
	for(auto b=0;b<64;b++)
	{
		for(auto c=0;c<64;c++)
		{
			stats[b][c]= cumul[b][c]/(float)loop;
			ok_bits+=(stats[b][c]==Approx(0.25).epsilon(0.08));
		}
	}
	REQUIRE(ok_bits>=56*56);
}