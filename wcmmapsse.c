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

  __m128i nll = _mm_set1_epi8(9 - 1);
  __m128i nlh = _mm_set1_epi8(13 + 1);
  __m128i sp = _mm_set1_epi8(' ');


  long long int words = -1;
  int endsp = 1, mask = 0;
  for (long long int i = 0; i < st.st_size; i += 16) {
    __m128i c = _mm_load_si128((__m128i *)(f + i));
    __m128i l = _mm_cmpgt_epi8(c, nll); /* at least TAB */
    __m128i h = _mm_cmpgt_epi8(nlh, c); /* at most CR */
    __m128i s = _mm_cmpeq_epi8(c, sp);  /* exactly SP */
    c = _mm_or_si128(s, _mm_and_si128(l, h));
    c = _mm_xor_si128(c, _mm_bslli_si128(c, 1));
    mask = _mm_movemask_epi8(c);
    int trans = _mm_popcnt_u32(mask);
    words += (trans / 2) + (endsp & ~mask);
    endsp = trans & 1;
  }
  words += (endsp & ~mask);
  printf("%lld\n", words);
}
