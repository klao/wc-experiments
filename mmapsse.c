#include <err.h>
#include <immintrin.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  struct stat st;
  if (fstat(0, &st) < 0) {
    err(1, NULL);
  }

  char *f = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
  if (f == MAP_FAILED) {
    err(1, NULL);
  }

  __m128i nls = _mm_set_epi32(0x0a0a0a0a, 0x0a0a0a0a, 0x0a0a0a0a, 0x0a0a0a0a);

  long long int checksum = 0;
  for (int i = 0; i < st.st_size; i += 16) {
    __m128i c = _mm_load_si128((__m128i *)(f + i));
    c = _mm_cmpeq_epi8(c, nls);
    unsigned int m = _mm_movemask_epi8(c);
    checksum += _mm_popcnt_u32(m);
  }
  printf("%lld\n", checksum);
}
