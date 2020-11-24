#include <stdio.h>
#include <unistd.h>

#define BUFSZ 16384

int main() {
  char buf[BUFSZ];
  long long int lines = 0;
  ssize_t n;

  while ((n = read(0, buf, BUFSZ)) > 0) {
    for (char *c = buf; c != buf + n; ++c) {
      if (*c == '\n') ++lines;
    }
  }
  printf("%lld\n", lines);
}
