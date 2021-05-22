# plf::rand
A C++ 03/11/etc replacement for rand()/srand() that's ~700% faster and typically has better statistical distribution. An adaptation of Melissa O'Neill's PCG rand (https://www.pcg-random.org/) with a fallback to xorshift generation for C++98/03 (which don't support the uint_64_t types which PCG generators require). See her page for more information and stats on the algorithm.

The two functions, plf::rand() and plf::srand(), correspond to rand()/srand() in terms of their parameters and return values.
