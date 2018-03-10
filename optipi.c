/* Compiling with: gcc -lm
  (might [not] be required) */

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// return values sometimes don't care (trash it)
uint64_t trash;

// pointer to help store results (THEN trash it)
uint64_t *trashpointer = &trash;

// 32bit 2-dimensional planar coordinates.
typedef struct {
  uint32_t p1;
  uint32_t p2;
} inputs_int32;

// temporarily promote to 64bit (prevent overflows)
typedef struct {
  uint64_t p1;
  uint64_t p2;
} outputs_int64;

inputs_int32 read_point(FILE *f);
outputs_int64 presanitizedmul(inputs_int32 unsafe32);
bool testquartercircle(FILE *f);

inputs_int32 read_point(FILE *f) {
  inputs_int32 randomspot;
  trash = fread(&randomspot, sizeof(inputs_int32), 1, f);
  return (inputs_int32)randomspot;
}

outputs_int64 presanitizedmul(inputs_int32 unsafe32) {
  outputs_int64 san64;
  san64.p1 = (uint64_t)unsafe32.p1 * (uint64_t)unsafe32.p1;
  san64.p2 = (uint64_t)unsafe32.p2 * (uint64_t)unsafe32.p2;
  return (outputs_int64)san64;
}

bool testquartercircle(FILE *f) {
  inputs_int32 checkit32 = (inputs_int32)read_point(f);
  outputs_int64 checkit64 = (outputs_int64)presanitizedmul(checkit32);

// gcc 7.x introduces a feature to check overflows.
bool returnbool = __builtin_add_overflow_p ((uint64_t)checkit64.p1,
  (uint64_t)checkit64.p2, (uint64_t)0);

  return (bool)returnbool;
}

int main(int argc, char** argv)
{
  FILE* f = fopen("/dev/urandom", "rb");
  uint64_t i=0, n=0;

  while (2) { // boolean would be fine.
    i++;
/* overflow = outside quarter circle:
   reverse test using !(not) operator */
    if (!testquartercircle(f))
      n++;
// output after 2^32 iterations
    if ((i & 0xffffffff) == 0) {
      double micropi = ((double)4000000.0 * (double)n) / (double)i;
      printf("%.6lf (x1000000)\n", (double)micropi);
      return 0;
    }
  }
}
