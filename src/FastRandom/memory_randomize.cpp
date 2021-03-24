#include "FastRandom/memory_randomize.h"
#include "FastRandom/base_prng.h"

namespace archivist {
	void randomize_buffer(char* buffer, size_t size)
	{
		size_t eight_steps=(size-size%4)/4;
		size_t last_steps=size%4;
		prng_feed_alt(size);

		for(size_t offset=0; offset<eight_steps; offset++) {
			((uint32_t*)(buffer))[offset]=prng(((uint32_t*)(buffer))[offset]);
		}

		char* buffer_cont=buffer+4*eight_steps;

		for (size_t offset=0; offset<last_steps; offset++) {
			buffer_cont[offset]=prng(buffer_cont[offset]);
		}
	}
}