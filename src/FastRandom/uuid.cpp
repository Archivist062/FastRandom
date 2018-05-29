/**
 * Licenced under MIT Licence
 *
 * (c) Ludovic 'Archivist' Lagouardette 2018
 *
 */

#include <FastRandom/uuid.h>
#include "catch.hpp"
#include <set>
#include <iostream>

thread_local std::random_device archivist::strong_uuid::src;

using namespace archivist;


#ifndef NO_CATCH
TEST_CASE("Test of fast UUIDs validity")
{
	const size_t loop=5000000;
	std::set<basic_uuid> rec;

	for(size_t lo=0; lo<loop; lo++) {
		fast_uuid m{};
		REQUIRE(rec.find(m)==rec.end());
		rec.insert(m);
	}
}


TEST_CASE("Test of fast UUIDs")
{
	const size_t loop=50000;
	auto begin = std::chrono::system_clock::now();

	for(size_t lo=0; lo<loop; lo++) {
		{
			fast_uuid m{};
		}
		{
			fast_uuid m{};
		}
		{
			fast_uuid m{};
		}
		{
			fast_uuid m{};
		}
		{
			fast_uuid m{};
		}
		{
			fast_uuid m{};
		}
		{
			fast_uuid m{};
		}
		{
			fast_uuid m{};
		}
		{
			fast_uuid m{};
		}
		{
			fast_uuid m{};
		}
	}

	auto end = std::chrono::system_clock::now();
	std::cout
	        << "FastUUID Performance: 1 every "
	        << std::chrono::duration_cast< std::chrono::nanoseconds >(
	            end-begin
	        ).count()/loop/10
	        << "ns"
	        << std::endl;
}

TEST_CASE("Test of strong UUIDs")
{
	const size_t loop=50000;
	auto begin = std::chrono::system_clock::now();

	for(size_t lo=0; lo<loop; lo++) {
		{
			strong_uuid m{};
		}
		{
			strong_uuid m{};
		}
		{
			strong_uuid m{};
		}
		{
			strong_uuid m{};
		}
		{
			strong_uuid m{};
		}
		{
			strong_uuid m{};
		}
		{
			strong_uuid m{};
		}
		{
			strong_uuid m{};
		}
		{
			strong_uuid m{};
		}
		{
			strong_uuid m{};
		}
	}

	auto end = std::chrono::system_clock::now();
	std::cout
	        << "Strong UUID Performance: 1 every "
	        << std::chrono::duration_cast< std::chrono::nanoseconds >(
	            end-begin
	        ).count()/loop/10
	        << "ns"
	        << std::endl;
}

TEST_CASE("Test of balanced UUIDs")
{
	const size_t loop=50000;
	auto begin = std::chrono::system_clock::now();

	for(size_t lo=0; lo<loop; lo++) {
		{
			balanced_uuid m{};
		}
		{
			balanced_uuid m{};
		}
		{
			balanced_uuid m{};
		}
		{
			balanced_uuid m{};
		}
		{
			balanced_uuid m{};
		}
		{
			balanced_uuid m{};
		}
		{
			balanced_uuid m{};
		}
		{
			balanced_uuid m{};
		}
		{
			balanced_uuid m{};
		}
		{
			balanced_uuid m{};
		}
	}

	auto end = std::chrono::system_clock::now();
	std::cout
	        << "Balanced UUID Performance: 1 every "
	        << std::chrono::duration_cast< std::chrono::nanoseconds >(
	            end-begin
	        ).count()/loop/10
	        << "ns"
	        << std::endl;
}
#endif