#include <err.h>
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

  // Do something with it, say sample every cache line:
  long long int checksum = 0;
  for (int i = 0; i < st.st_size; i += 64) {
    checksum += *((long long int *)(f + i));
  }
  printf("%lld\n", checksum);
}
