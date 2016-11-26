#include <stdio.h>
#include <stdint.h>
#include <math.h>

static __inline__ uint64_t rdtsc(void) {
  unsigned lo, hi;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ((uint64_t)lo) | (((uint64_t)hi) << 32);
}

int main() {
    uint64_t max_steps = 64 * 1024 * 1024;
    uint64_t start, end;

    int steps = 1024;
    int arr[1024 * 1024];
    int lengthMod = (1024 * 1024) - 1;
    int i, x;

    for (x = 0; x < 26; x++) {
      start = rdtsc();
      for (i = 0; i < max_steps; i++) {
          arr[(i * (steps*(uint64_t)pow(2, x))) & lengthMod]++;
      }
      end = rdtsc();
      printf("Access time for stride size of %llu: %llu cycles \n", steps*(uint64_t)pow(2,x), end-start);
    }
    return 0;
}
