#ifndef archivist_uuid_h
#define archivist_uuid_h

/**
 * Licenced under MIT Licence
 *
 * (c) Ludovic 'Archivist' Lagouardette 2018
 *
 */

#include "FastRandom/base_prng.h"
#include <chrono>
#include <iostream>
#include <string>
#include <random>

namespace archivist
{

   enum class uuid_modes
   {
      balanced, fast, strong
   };

   struct basic_uuid
   {
      static thread_local std::random_device src;
      static thread_local uint8_t balance;
      uint64_t state[2]{};

      inline void init_default()
      {
         state[0] = prng(9998);
         state[1] = prng(17);
      }

      inline void init_strong()
      {
         uint32_t internal = src();
         uint32_t cross = src() | std::chrono::duration_cast< std::chrono::microseconds >(
                                  std::chrono::system_clock::now().time_since_epoch()).count();

         state[0] = prng(internal);
         prng_feed(cross);
         state[1] = prng(cross);
      }

      bool operator==(const basic_uuid& rhs) const
      {
         return (state[0] == rhs.state[0]) && (state[1] == rhs.state[1]);
      }

      bool operator<(const basic_uuid& rhs) const
      {
         return (state[0] < rhs.state[0]) || ((state[1] < rhs.state[1]) && (state[0] == rhs.state[0]));
      }
   };

   using uuid = archivist::basic_uuid;

   struct uuid_balanced : public basic_uuid
   {
      uuid_balanced()
      {
         balance++;
         if(balance % 128)
            init_default();

         else
            init_strong();
      }
   };

   struct uuid_fast : public basic_uuid
   {
      uuid_fast()
      {
         init_default();
      }
   };

   struct uuid_strong : public basic_uuid
   {
      uuid_strong()
      {
         init_strong();
      }
   };

}

#endif//archivist_uuid_h
