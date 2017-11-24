// Compile with: gcc -lm

// 0xFFFFFFFFFFFFFFFF is easily typo'd
#define bigint64 UINT64_MAX;

#include <math.h>
#include <stdio.h>
#include <stdint.h>

// return value for fread is ignored (trash it)
uint64_t trash;

// 64bit 9-dimensional hyperspace coordinates.
typedef struct {
  uint64_t p1;
  uint64_t p2;
  uint64_t p3;
  uint64_t p4;
  uint64_t p5;
  uint64_t p6;
  uint64_t p7;
  uint64_t p8;
  uint64_t p9;
} hyperspot_int64;

// extended precision (80bit) floating point coordinates
typedef struct {
  __float80 p1;
  __float80 p2;
  __float80 p3;
  __float80 p4;
  __float80 p5;
  __float80 p6;
  __float80 p7;
  __float80 p8;
  __float80 p9;
} hyperspot_fp80;

hyperspot_int64 read_hyperpoint(FILE *f);
hyperspot_fp80 hyperfloatify(hyperspot_int64 hi64);
__float80 measurehypersphere(FILE *f);

hyperspot_int64 read_hyperpoint(FILE *f) {
  hyperspot_int64 randomspot;
  trash = fread(&randomspot, sizeof(hyperspot_int64), 1, f);
  return (hyperspot_int64)randomspot;
}

hyperspot_fp80 hyperfloatify(hyperspot_int64 hi64) {
  // extended precision (80bit) floating point
  const __float80 bigfloat80 = (__float80)1.0 * bigint64;
  hyperspot_fp80 hf80;
  hf80.p1 = hi64.p1 / bigfloat80;
  hf80.p2 = hi64.p2 / bigfloat80;
  hf80.p3 = hi64.p3 / bigfloat80;
  hf80.p4 = hi64.p4 / bigfloat80;
  hf80.p5 = hi64.p5 / bigfloat80;
  hf80.p6 = hi64.p6 / bigfloat80;
  hf80.p7 = hi64.p7 / bigfloat80;
  hf80.p8 = hi64.p8 / bigfloat80;
  hf80.p9 = hi64.p9 / bigfloat80;
  return (hyperspot_fp80)hf80;
}

__float80 measurehypersphere(FILE *f) {
  hyperspot_int64 checkit64 = (hyperspot_int64)read_hyperpoint(f);
  hyperspot_fp80 checkit80 = (hyperspot_fp80)hyperfloatify(checkit64);
  __float80 distance80 = (__float80)(
    (checkit80.p1 * checkit80.p1) +
    (checkit80.p2 * checkit80.p2) +
    (checkit80.p3 * checkit80.p3) +
    (checkit80.p4 * checkit80.p4) +
    (checkit80.p5 * checkit80.p5) +
    (checkit80.p6 * checkit80.p6) +
    (checkit80.p7 * checkit80.p7) +
    (checkit80.p8 * checkit80.p8) +
    (checkit80.p9 * checkit80.p9)
  );
  return (__float80)distance80;
}

int main(int argc, char** argv)
{
  FILE* f = fopen("/dev/urandom", "rb");
  uint64_t i=0, n=0;
  while (2) { // any nonzero value is TRUE;
    i++;
    if (measurehypersphere(f) < 1.0)
      n++;
    // output after 2^18 iterations
    if ((i & 0x03ffff) == 0) {
      __float80 pi = pow(
        (15120.0 * (
            (__float80)n / (__float80)i
          )
        ), (1/4)); // 4th root
      printf("%.17Lf\n", (__float80)pi);
      return 0;
    }
  }
}
