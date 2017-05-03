// -*- mode: c; c-file-style: "linux" -*-
//
// Monte Carlo estimate of pi optimized to use fixed-point arithmetics
// instead of FPU. By zouppen@gmail.com
//
// Compile with `gcc --std=gnu99 -Wall -o montecarlo montecarlo.c`

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

uint32_t read_point(FILE *f);
bool sample(FILE *f);

// Returns a random integer between 0 and 2^31.5.
uint32_t read_point(FILE *f)
{
	uint32_t val;
	do {
		fread(&val, 4, 1, f);
	} while (val > 3037000499);
	return val;
}

// Do Monte carlo sampling to a circle with radius of 2^31.5. When
// squared is exactly 2^63 and fits inside 64 bit integer.
bool sample(FILE *f) {
	uint64_t a = read_point(f);
	uint64_t b = read_point(f);
	return (a*a + b*b) < ((uint64_t)1<<63);
}

int main(int argc, char** argv)
{
	// Process command line
	if (argc != 2) {
		fprintf(stderr, "Usage: %s ITERATIONS\n", argv[0]);
		return 1;
	}
	uint64_t m = strtoll(argv[1], NULL, 10);

	// Use Linux kernel random numbers
	FILE* f = fopen("/dev/urandom", "rb");

	// Sample n times and test how many times we hit the circle.
	uint64_t n = 0;
	for (uint64_t i=0; i<m; i++) {
		if (sample(f)) {
			n++;
		}
	}
	double pi = 4.0 * n / m;
	printf("%f\n", pi);
}

	
