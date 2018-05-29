/**
 * Licenced under MIT Licence
 *
 * (c) Ludovic 'Archivist' Lagouardette 2018
 *
 */

#include "FastRandom/base_prng.h"
#include <random>
#include <set>
#include <iostream>
#ifndef NO_CATCH
#include "catch.hpp"
#endif

namespace archivist {
		#ifndef COMPAT_TLS
			thread_local uint64_t internal::prngState;
			thread_local uint64_t internal::prngState_a;
			thread_local uint64_t internal::prngState_c;
			thread_local uint8_t internal::balance;
			thread_local std::mt19937_64 internal_rnd;
		#else
			 std::atomic<uint64_t> internal::prngState;
			 std::atomic<uint64_t> internal::prngState_c;
			 std::atomic<uint64_t> internal::prngState_a;
			 std::atomic<uint8_t> internal::balance;
			 std::mt19937_64 internal_rnd;
			 std::mutex rnd_guard;
		#endif // COMPAT_TLS
	std::atomic<uint64_t> internal::_entropy_contributor;

	uint64_t internal::rnd()
	{
		#ifndef COMPAT_TLS
			return internal_rnd();
		#else
			rnd_guard.lock();
			auto n = internal_rnd();
			rnd_guard.unlock();
			return n;
		#endif // COMPAT_TLS
	}

	class dummy168783486732 {
	public:
		dummy168783486732()
		{
			std::random_device rnd;
			internal::prngState_a=rnd()*0x0000000100000000+rnd();
			internal::prngState_c=0x7A7A6565655B5BULL;
			internal::prngState=0x7A7A7A7A5B5B5B5BULL;
			internal::balance=0;
			prng_feed_alt(rnd()*0x0000000100000000+rnd());
		#ifndef COMPAT_TLS
			internal_rnd.seed(rnd()*0x0000000100000000+rnd());
		#else
			rnd_guard.lock();
			internal_rnd.seed(rnd()*0x0000000100000000+rnd());
			rnd_guard.unlock();
		#endif // COMPAT_TLS
		}
	};
	#ifndef COMPAT_TLS
	thread_local 
	#endif // COMPAT_TLS
		dummy168783486732 dummy168783486732_inst;
}
#ifndef NO_CATCH
TEST_CASE("Testing the PRNG basic generator")
{
	const size_t loop = 5000;
	std::set<uint64_t> rec;

	for(size_t i=0; i<loop; i++) {
		uint64_t data=archivist::prng();
		REQUIRE(rec.find(data)==rec.end());
		rec.insert(data);
	}
}

TEST_CASE("Test of PRNG speed")
{
	const size_t loop=50000;
	auto begin = std::chrono::system_clock::now();

	for(size_t lo=0; lo<loop; lo++) {
		archivist::prng();
		archivist::prng();
		archivist::prng();
		archivist::prng();
		archivist::prng();
		archivist::prng();
		archivist::prng();
		archivist::prng();
		archivist::prng();
		archivist::prng();
	}

	auto end = std::chrono::system_clock::now();
	std::cout
	        << "PRNG Performance: 1 every "
	        << std::chrono::duration_cast< std::chrono::nanoseconds >(
	            end-begin
	        ).count()/loop/10
	        << "ns"
	        << std::endl;
}

TEST_CASE("Test of auto fed PRNG speed")
{
	const size_t loop=50000;
	auto begin = std::chrono::system_clock::now();

	for(size_t lo=0; lo<loop; lo++) {
		archivist::prng_auto_feed();
		archivist::prng_auto_feed();
		archivist::prng_auto_feed();
		archivist::prng_auto_feed();
		archivist::prng_auto_feed();
		archivist::prng_auto_feed();
		archivist::prng_auto_feed();
		archivist::prng_auto_feed();
		archivist::prng_auto_feed();
		archivist::prng_auto_feed();
	}

	auto end = std::chrono::system_clock::now();
	std::cout
	        << "Autofed PRNG Performance: 1 every "
	        << std::chrono::duration_cast< std::chrono::nanoseconds >(
	            end-begin
	        ).count()/loop/10
	        << "ns"
	        << std::endl;
}

TEST_CASE("Testing the PRNG basic generator (statistics)")
{
	const size_t loop = 5000000;
	std::set<uint64_t> rec;

	for(size_t i=0; i<loop; i++) {
		uint64_t data=archivist::prng();
		rec.insert(data);
	}

	int cumul[64]= {};

	for(auto val : rec) {
		for(auto b=0; b<64; b++) {
			cumul[b]+= ((1<<b)&val)!=0;
		}
	}

	float stats[64]= {};
	int ok_bits=0;

	std::cout<<"Probability stats:\n";

	for(auto b=0; b<64; b++) {
		stats[b]= cumul[b]/(float)loop;
		std::cout<<"b"<<b<<":"<<stats[b]<<"\t";

		if(b>0 && b%8==0) {
			std::cout<<"\n";
		}

		ok_bits+=(stats[b]==Approx(0.5).epsilon(0.08));
	}

	std::cout<<"\nTotal of ok_bits:"<<ok_bits<<std::endl;
	REQUIRE(ok_bits>=56);
}

TEST_CASE("Testing the PRNG basic generator (statistics 2)")
{
	const size_t loop = 5000000;
	std::set<uint64_t> rec;

	for(size_t i=0; i<loop; i++) {
		uint64_t data=archivist::prng();
		rec.insert(data);
	}

	int cumul[64][64]= {};

	for(auto val : rec) {
		for(auto b=0; b<64; b++) {
			for(auto c=0; c<64; c++) {
				cumul[b][c]+= (((1<<b)&val)!=0) && (((1<<c)&val)!=0);
			}
		}
	}

	float stats[64][64]= {};
	int ok_bits=0;

	for(auto b=0; b<64; b++) {
		for(auto c=0; c<64; c++) {
			stats[b][c]= cumul[b][c]/(float)loop;
			ok_bits+=(stats[b][c]==Approx(0.25).epsilon(0.08));
		}
	}

	std::cout<<"Total of independantly ok_bits:"<<ok_bits<<std::endl;

	REQUIRE(ok_bits>=56*56);
}

TEST_CASE("Testing the PRNG auto fed generator (statistics)")
{
	const size_t loop = 5000000;
	std::set<uint64_t> rec;

	for(size_t i=0; i<loop; i++) {
		uint64_t data=archivist::prng_auto_feed();
		rec.insert(data);
	}

	int cumul[64]= {};

	for(auto val : rec) {
		for(auto b=0; b<64; b++) {
			cumul[b]+= ((1<<b)&val)!=0;
		}
	}

	float stats[64]= {};
	int ok_bits=0;

	std::cout<<"Probability stats (autofeed):\n";

	for(auto b=0; b<64; b++) {
		stats[b]= cumul[b]/(float)loop;
		std::cout<<"b"<<b<<":"<<stats[b]<<"\t";

		if(b>0 && b%8==0) {
			std::cout<<"\n";
		}

		ok_bits+=(stats[b]==Approx(0.5).epsilon(0.08));
	}

	std::cout<<"\nTotal of ok_bits (autofeed):"<<ok_bits<<std::endl;
	REQUIRE(ok_bits>=60);
}

TEST_CASE("Testing the PRNG auto fed generator (statistics 2)")
{
	const size_t loop = 5000000;
	std::set<uint64_t> rec;

	for(size_t i=0; i<loop; i++) {
		uint64_t data=archivist::prng_auto_feed();
		rec.insert(data);
	}

	int cumul[64][64]= {};

	for(auto val : rec) {
		for(auto b=0; b<64; b++) {
			for(auto c=0; c<64; c++) {
				cumul[b][c]+= (((1<<b)&val)!=0) && (((1<<c)&val)!=0);
			}
		}
	}

	float stats[64][64]= {};
	int ok_bits=0;

	for(auto b=0; b<64; b++) {
		for(auto c=0; c<64; c++) {
			stats[b][c]= cumul[b][c]/(float)loop;
			ok_bits+=(stats[b][c]==Approx(0.25).epsilon(0.08));
		}
	}

	std::cout<<"Total of independantly ok_bits (autofeed):"<<ok_bits<<std::endl;

	REQUIRE(ok_bits>=60*60);
}
#endif