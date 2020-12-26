# plf_pcgrand
A C++ 03/11/etc replacement for rand()/srand() that's ~700% faster and typically has better statistical distribution. An adaption of Melissa O'Neill's PCG rand (https://www.pcg-random.org/). See her page for more information and stats on the algorithm.

The two functions, plf::pcg_rand() and plf::pcg_srand(), correspond to rand()/srand() in terms of their parameters and return values.
