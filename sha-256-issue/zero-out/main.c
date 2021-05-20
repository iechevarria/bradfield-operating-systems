#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sha-256.h"

void _print_hash(uint8_t hash[32]) {
  for (int i = 0; i < 32; i++)
    printf("%02x", hash[i]);
}

void run(int trash) {
  uint8_t hash[32];
  uint32_t x, sum;

  sum = 2;

  for (int i = 0; i < 5; i++) {
    memset(hash, 0x00, sizeof hash);
    calc_sha_256(hash, (const void *)(&sum), 32);
    printf("sha256(%u) = ", sum);
    _print_hash(hash);
    putchar('\n');
  }
}

int main() {
  // value passed to run is ignored
  run(-1);
}
