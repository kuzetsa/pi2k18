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
// squared is exactly 2^62 and fits inside 64 bit integer.
bool sample(FILE *f) {
	uint64_t a = read_point(f);
	uint64_t b = read_point(f);
	return (a*a + b*b) < ((uint64_t)1<<62);
}

int main(int argc, char** argv)
{
	// Use Linux kernel random numbers
	FILE* f = fopen("/dev/urandom", "rb");

	// Sample and test how many times we hit the circle.
	uint64_t i=0, n=0;
	while (true) {
		i++;

		if (sample(f)) {
			n++;
		}

		// Print only periodically
		if ((i & 0x1ff) == 0) {
			double pi = 4.0 * n / i;
			printf("After %10lu iterations: %.10f\n", i, pi);
		}
	}
}
