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
  // printf("file size: %ld\n", st.st_size);

  void *f = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
  if (f == MAP_FAILED) {
    err(1, NULL);
  }

  // Do something with it.
  long long int checksum = 0;
  for (int i = 0; i < st.st_size / 32; ++i) {
    // sample every cache line:
    checksum += ((long long int *)f)[i];
  }
  printf("%lld\n", checksum);
}
