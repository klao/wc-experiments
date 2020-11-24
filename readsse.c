#include <err.h>
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSZ (1 << 17)
#define ALIGN (32 - 1)

int main() {
  char *buf = malloc(BUFSZ + 40);
  if ((size_t) buf & ALIGN) {
    warnx("misaligned alloc: %p", buf);
    buf = (char *) (~ALIGN & ((size_t) buf + ALIGN));
    warnx("aligned: %p", buf);
  }

  long long int lines = 0;

  ssize_t n;
  __m128i nls = _mm_set_epi32(0x0a0a0a0a, 0x0a0a0a0a, 0x0a0a0a0a, 0x0a0a0a0a);

  while ((n = read(0, buf, BUFSZ)) > 0) {
    if (n & ALIGN) {
      memset(buf + n, 0, ALIGN + 1);
    }
    for (int i = 0; i < n; i += 16) {
      __m128i c = _mm_load_si128((__m128i *)(buf + i));
      c = _mm_cmpeq_epi8(c, nls);
      unsigned int m = _mm_movemask_epi8(c);
      lines += _mm_popcnt_u32(m);
    }
  }
  printf("%lld\n", lines);
}
