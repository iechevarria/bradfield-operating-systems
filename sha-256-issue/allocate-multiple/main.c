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
  uint8_t hash1[32];
  uint8_t hash2[32];
  uint8_t hash3[32];
  uint8_t hash4[32];
  uint8_t hash5[32];

  uint32_t x, sum;

  sum = 2;

  calc_sha_256(hash1, (const void *)(&sum), 32);
  printf("sha256(%u) = ", sum);
  _print_hash(hash1);
  putchar('\n');

  calc_sha_256(hash2, (const void *)(&sum), 32);
  printf("sha256(%u) = ", sum);
  _print_hash(hash2);
  putchar('\n');

  calc_sha_256(hash3, (const void *)(&sum), 32);
  printf("sha256(%u) = ", sum);
  _print_hash(hash3);
  putchar('\n');

  calc_sha_256(hash4, (const void *)(&sum), 32);
  printf("sha256(%u) = ", sum);
  _print_hash(hash4);
  putchar('\n');

  calc_sha_256(hash5, (const void *)(&sum), 32);
  printf("sha256(%u) = ", sum);
  _print_hash(hash5);
  putchar('\n');
}

int main() {
  // value passed to run is ignored
  run(-1);
}
