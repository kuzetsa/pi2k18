// Compile with: gcc -lm

#define big64 UINT64_MAX;

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

uint64_t read_point(FILE *f);
bool sample(FILE *f);

uint64_t read_point(FILE *f) {
  uint64_t val;
  uint16_t trash = fread(&val, 8, 1, f);
  return (uint64_t)val;
}

bool sample(FILE *f) {
  __float80 double64 = (__float80)1.0 * big64;
  __float80 a = (__float80)read_point(f) / double64;
  __float80 b = (__float80)read_point(f) / double64;
  __float80 c = (__float80)read_point(f) / double64;
  __float80 d = (__float80)read_point(f) / double64;
  __float80 e = (__float80)read_point(f) / double64;
  __float80 k = (__float80)read_point(f) / double64;
  __float80 g = (__float80)read_point(f) / double64;
  __float80 h = (__float80)read_point(f) / double64;
  __float80 j = (__float80)read_point(f) / double64;
  return (a*a + b*b + c*c + d*d + e*e + k*k + g*g + h*h + j*j) < (__float80)1.0;
}

int main(int argc, char** argv)
{
  FILE* f = fopen("/dev/urandom", "rb");

  uint64_t i=0, n=0;
  while (true) {
    i++;

    if (sample(f))
      n++;

    // exit after 2^20 iterations
    if ((i & 0x0fffff) == 0) {
      __float80 pi = sqrt(sqrt(15120.0 * ((__float80)n / (__float80)i)));
      printf("%.17Lf\n", (__float80)pi);
      return 0;
    }
  }
}
