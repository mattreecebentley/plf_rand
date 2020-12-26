/*
 * PCG Random Number Generation for C.
 *
 * Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
 *
*/

// Simplified by Matt Bentley 2020 (mattreecebentley@gmail.com)

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


#ifndef PLF_PCGRAND
#define PLF_PCGRAND

#if defined(_MSC_VER)
	#if _MSC_VER >= 1900
		#define PLF_PCGRAND_NOEXCEPT noexcept
	#else
		#define PLF_PCGRAND_NOEXCEPT throw()
	#endif
#elif defined(__cplusplus) && __cplusplus >= 201103L // C++11 support, at least
	#if defined(__GNUC__) && defined(__GNUC_MINOR__) && !defined(__clang__) // If compiler is GCC/G++
		#if (__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || __GNUC__ > 4
			#define PLF_PCGRAND_NOEXCEPT noexcept
		#else
			#define PLF_PCGRAND_NOEXCEPT throw()
		#endif
	#elif defined(__clang__)
		#if __has_feature(cxx_noexcept)
			#define PLF_PCGRAND_NOEXCEPT noexcept
		#else
			#define PLF_PCGRAND_NOEXCEPT throw()
		#endif
	#else // Assume support for other compilers
		#define PLF_PCGRAND_NOEXCEPT noexcept
	#endif
#else
	#define PLF_PCGRAND_NOEXCEPT throw()
#endif


#if defined(__cplusplus) && __cplusplus >= 201103L
	#include <cstdint>
	#define PLF_PCGRAND_U32 uint32_t
	#define PLF_PCGRAND_U64 uint64_t
#else
	#include <cstddef>
	#define PLF_PCGRAND_U32 unsigned int
	#define PLF_PCGRAND_U64 size_t
#endif



namespace plf
{

struct pcg_state
{
    PLF_PCGRAND_U64 state;             // RNG state.  All values are possible.
    PLF_PCGRAND_U64 seq;               // Controls which RNG sequence (stream) is selected. Must *always* be odd.
} static pcg_global = {0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL};



PLF_PCGRAND_U32 pcg_rand() PLF_PCGRAND_NOEXCEPT
{
    const PLF_PCGRAND_U64 oldstate = pcg_global.state;
    pcg_global.state = oldstate * 6364136223846793005ULL + pcg_global.seq;
    const PLF_PCGRAND_U32 xorshifted = static_cast<PLF_PCGRAND_U32>(((oldstate >> 18u) ^ oldstate) >> 27u);
    const PLF_PCGRAND_U32 rot = static_cast<PLF_PCGRAND_U32>(oldstate >> 59u);
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}



void pcg_srand(const PLF_PCGRAND_U32 init) PLF_PCGRAND_NOEXCEPT
{
    pcg_global.state = 0x853c49e6748fea9bULL;
    pcg_global.seq = (init << 1u) | 1u;
    pcg_rand();
    pcg_global.state += 0x853c49e6748fea9bULL;
    pcg_rand();
}

} // namespace

#undef PLF_PCGRAND_NOEXCEPT
#undef PLF_PCGRAND_U64
#undef PLF_PCGRAND_U32

#endif // PLF_PCGRAND