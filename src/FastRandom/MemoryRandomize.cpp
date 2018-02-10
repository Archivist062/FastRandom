#include "FastRandom/MemoryRandomize.h"
#include "FastRandom/Base_PRNG.h"

namespace archivist{
	void randomize_buffer(char* buffer, size_t size)
	{
		size_t eight_steps=(size-size%8)/8;
		size_t last_steps=size%8;
		PRNG_feed_alt(size);
		for(size_t offset=0;offset<eight_steps;offset++)
		{
			((uint64_t*)(buffer))[offset]=PRNG(((uint64_t*)(buffer))[offset]);
		}
		char* buffer_cont=buffer+8*eight_steps;
		for (size_t offset=0;offset<last_steps;offset++)
		{
			buffer_cont[offset]=PRNG(buffer_cont[offset]);
		}
	}
}