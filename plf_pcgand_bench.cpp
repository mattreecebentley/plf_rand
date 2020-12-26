#include "plf_nanotimer.h"
#include "plf_pcgrand.h"
#include <cstdio>

int main(int argc, char **argv)
{
	using namespace plf;

	double total_time = 0, total_time_2 = 0, total_time_3 = 0, sum = 0;
	nanotimer the_timer;


	for (char loop = 0; loop != 10; ++loop)
	{
		// time rand:
		millisecond_delay(1000);
		the_timer.start();

		for (unsigned int count = 0; count != 300000000; ++count)
		{
			sum += rand() & 255;
		}

		total_time += the_timer.get_elapsed_ms();


		// time pcg_rand:
		millisecond_delay(1000);
		the_timer.start();

		for (unsigned int count = 0; count != 300000000; ++count)
		{
			sum += pcg_rand() & 255;
		}

		total_time_2 += the_timer.get_elapsed_ms();
	}


	printf("Sum: %g, rand() time: %gms, pcg_rand() time: %gms\n", sum, total_time, total_time_2); // output sum otherwise gcc/clang may optimize out loops

	return 0;
}