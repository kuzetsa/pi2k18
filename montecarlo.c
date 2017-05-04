// -*- mode: c; c-file-style: "linux" -*-
//
// Monte Carlo estimate of pi optimized to use fixed-point arithmetics
// instead of FPU. By zouppen@gmail.com
//
// NB! I left error checks away for clarity. Never access files in
// real life without checking return values!!
//
// Compile with `gcc --std=gnu99 -Wall -o montecarlo montecarlo.c`

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

uint32_t read_point(FILE *f);
bool sample(FILE *f);

// Returns a random integer between 0 and 2^31.
uint32_t read_point(FILE *f)
{
	uint32_t val;
 	fread(&val, 4, 1, f);
	return val & ~(1<<31);
}

// Do Monte carlo sampling to a circle with radius of 2^31. When
// squared and summed we get number which is always less than 2^63
// which fits inside a 64 bit unsigned integer.
bool sample(FILE *f) {
	uint64_t a = read_point(f);
	uint64_t b = read_point(f);
	return (a*a + b*b) < ((uint64_t)1<<62);
}

int main(int argc, char** argv)
{
	// Use Linux kernel random numbers
	FILE* f = fopen("/dev/urandom", "rb");

	uint64_t i=0, n=0;
	while (true) {
		i++;

		// Sample and test if we hit inside a circle.
		if (sample(f)) {
			n++;
		}

		// Print only periodically (once per 512 iterations)
		if ((i & 0x1ff) == 0) {
			// We have sampled quarter circle so we need
			// to multiply by 4 to get the value of pi.
			double pi = 4.0 * n / i;
			printf("After %10lu iterations: %.10f\n", i, pi);
		}
	}
}
