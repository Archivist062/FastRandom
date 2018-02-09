/**
 * Licenced under MIT Licence
 *  
 * (c) Ludovic 'Archivist' Lagouardette 2018
 *
 */

 #include <FastRandom/UUID.h>
#include "catch.hpp"
#include <set>
#include <iostream>

thread_local std::random_device archivist::UUID::src;
thread_local uint8_t archivist::UUID::balance;

using namespace archivist;


TEST_CASE("Test of fast UUIDs validity")
{
	const size_t loop=5000000;
	std::set<UUID> rec;
	for(size_t lo=0;lo<loop;lo++)
	{
		UUID m{fast_uuid()};
		REQUIRE(rec.find(m)==rec.end());
		rec.insert(m);
	}
}


TEST_CASE("Test of fast UUIDs")
{
	const size_t loop=50000;
	auto begin = std::chrono::system_clock::now();
	for(size_t lo=0;lo<loop;lo++)
	{
		{UUID m{fast_uuid()};}
		{UUID m{fast_uuid()};}
		{UUID m{fast_uuid()};}
		{UUID m{fast_uuid()};}
		{UUID m{fast_uuid()};}

		{UUID m{fast_uuid()};}
		{UUID m{fast_uuid()};}
		{UUID m{fast_uuid()};}
		{UUID m{fast_uuid()};}
		{UUID m{fast_uuid()};}
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
	for(size_t lo=0;lo<loop;lo++)
	{
		{UUID m{strong_uuid()};}
		{UUID m{strong_uuid()};}
		{UUID m{strong_uuid()};}
		{UUID m{strong_uuid()};}
		{UUID m{strong_uuid()};}

		{UUID m{strong_uuid()};}
		{UUID m{strong_uuid()};}
		{UUID m{strong_uuid()};}
		{UUID m{strong_uuid()};}
		{UUID m{strong_uuid()};}
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
	for(size_t lo=0;lo<loop;lo++)
	{
		{UUID m{balanced_uuid()};}
		{UUID m{balanced_uuid()};}
		{UUID m{balanced_uuid()};}
		{UUID m{balanced_uuid()};}
		{UUID m{balanced_uuid()};}

		{UUID m{balanced_uuid()};}
		{UUID m{balanced_uuid()};}
		{UUID m{balanced_uuid()};}
		{UUID m{balanced_uuid()};}
		{UUID m{balanced_uuid()};}
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