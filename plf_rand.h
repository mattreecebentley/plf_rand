// zLib license (https://www.zlib.net/zlib_license.html):
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
// 	claim that you wrote the original software. If you use this software
// 	in a product, an acknowledgement in the product documentation would be
// 	appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
// 	misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.


#ifndef PLF_RAND_H
#define PLF_RAND_H

#if defined(_MSC_VER)
	#if _MSC_VER >= 1900
		#define PLF_NOEXCEPT noexcept
	#else
		#define PLF_NOEXCEPT throw()
	#endif
#elif defined(__cplusplus) && __cplusplus >= 201103L // C++11 support, at least
	#if defined(__GNUC__) && defined(__GNUC_MINOR__) && !defined(__clang__) // If compiler is GCC/G++
		#if (__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || __GNUC__ > 4
			#define PLF_NOEXCEPT noexcept
		#else
			#define PLF_NOEXCEPT throw()
		#endif
	#elif defined(__clang__)
		#if __has_feature(cxx_noexcept)
			#define PLF_NOEXCEPT noexcept
		#else
			#define PLF_NOEXCEPT throw()
		#endif
	#else // Assume support for other compilers
		#define PLF_NOEXCEPT noexcept
	#endif
#else
	#define PLF_NOEXCEPT throw()
#endif



#if defined(__cplusplus) && __cplusplus >= 201103L

/*
 * PCG Random Number Generation for C.
 *
 * Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
 *
*/

// Simplified by Matt Bentley 2021 (mattreecebentley@gmail.com)

#include <cstdint>

namespace plf
{

struct pcg_state
{
    uint_least64_t state;             // RNG state.  All values are possible.
    uint_least64_t seq;               // Controls which RNG sequence (stream) is selected. Must *always* be odd.
} static pcg_global = {0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL};



unsigned int rand() PLF_NOEXCEPT
{
    const uint_least64_t oldstate = pcg_global.state;
    pcg_global.state = oldstate * 6364136223846793005ULL + pcg_global.seq;
    const uint_least32_t xorshifted = static_cast<uint_least32_t>(((oldstate >> 18u) ^ oldstate) >> 27u);
    const uint_least32_t rot = static_cast<uint_least32_t>(oldstate >> 59u);
    return static_cast<unsigned int>((xorshifted >> rot) | (xorshifted << ((-rot) & 31)));
}



void srand(const unsigned int init) PLF_NOEXCEPT
{
    pcg_global.state = 0x853c49e6748fea9bULL;
    pcg_global.seq = (static_cast<uint_least32_t>(init) << 1u) | 1u;
    plf::rand();
    pcg_global.state += 0x853c49e6748fea9bULL;
    plf::rand();
}

} // namespace


#else // not C++11 or higher

// xor_shift128++ generator substituted for pre-C++11 compilers (since C++03 doesn't have guaranteed cross-compiler 64-bit unsigned ints).
// Based on https://codingforspeed.com/using-faster-psudo-random-generator-xorshift/

namespace plf
{

// unsigned long is at least 32 bits in C++ - unsigned int is only guaranteed to be at least 16 bits:
static unsigned long xorand_nums[4] = {123456789, 362436069, 521288629, 88675123};


unsigned int rand() PLF_NOEXCEPT
{
	const unsigned long temp = xorand_nums[0] ^ (xorand_nums[0] << 11);

	// Rotate the static values ([3] rotation in return statement):
	xorand_nums[0] = xorand_nums[1];
	xorand_nums[1] = xorand_nums[2];
	xorand_nums[2] = xorand_nums[3];

	return static_cast<unsigned int>(xorand_nums[3] = xorand_nums[3] ^ (xorand_nums[3] >> 19) ^ (temp ^ (temp >> 8)));
}


void srand(const unsigned int seed) PLF_NOEXCEPT
{
	xorand_nums[0] = 123456789 + seed;
	xorand_nums[1] = 362436069 + seed;
	xorand_nums[2] = 521288629 + seed;
	xorand_nums[3] = 88675123 + seed;

	for (unsigned char index = 0; index != 4; ++index)
	{
		if (xorand_nums[index] == 0)
		{
			xorand_nums[index] = 1;
		}
	}
}

}

#endif


#undef PLF_NOEXCEPT

#endif // PLF_RAND_H