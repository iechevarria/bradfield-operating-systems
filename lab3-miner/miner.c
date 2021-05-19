#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sha-256.h"

#define DIFFICULTY 2 // number of bytes of leading 0's
#define WORKER_COUNT 1 // number of worker threads
#define BUF_SIZE 256 // number of entries in ring buffer

//
// thread stuff
//

pthread_t thread_ids[WORKER_COUNT];
int successes[WORKER_COUNT];

// how do you do this the right way?
// this is so so so dumb
int stupid_way_to_pass_int[4] = {1, 2, 3, 4};

uint32_t nonce;

//
// ring buffer stuff
//

struct mine_result {
  uint32_t value;
  int worker;
};

struct mine_result buffer[BUF_SIZE];
int wp = 0;
int rp = 0;
sem_t *readable;
sem_t *writable;

pthread_mutex_t lock;


void _print_hash(uint8_t hash[32]) {
  for (int i = 0; i < 32; i++)
    printf("%02x", hash[i]);
}

int successful(uint8_t hash[32]) {
  for (int i = 0; i < DIFFICULTY; i++)
    if (hash[i] != 0)
      return 0;
  return 1;
}

// TODO instead of just printing, communicate the successful
// hash value back once found
void hash_and_print() {


  // printf("Found summand to be %u, since sha256(%u + %u) = ", x, nonce, x);
  // _print_hash(hash);
  // printf(", after %d attempts.\n", attempts);
}

void *thread_func(void *id) {
  int worker = *(int *) id;

  uint8_t hash[32];
  uint32_t x, sum;

  // do the work
  while (1) {
    do {
      x = rand();
      sum = nonce + x;
      memset(hash, 0x00, sizeof hash);
      calc_sha_256(hash, (const void *)(&sum), 32U);
    } while (!successful(hash));

  
    struct mine_result to_write = {.value=x, .worker=worker};
    _print_hash(hash);
    printf("   sum: %u\n", sum);

    memset(hash, 0x00, sizeof hash);
    calc_sha_256(hash, (const void *)(&sum), 32U);
    _print_hash(hash);
    printf("   sum: %u\n", sum);

    sem_wait(writable);

    pthread_mutex_lock(&lock);
    buffer[wp] = to_write;
    wp = (wp + 1) % BUF_SIZE;
    pthread_mutex_unlock(&lock);

    sem_post(readable);
  }
  
  return NULL;
}

int main() {
  // TODO instead of just running this on one nonce, generate
  // a sequence of random nonces and have miners hash the nonce
  // separately in threads

  sem_unlink("/tmp/bsem1");
  sem_unlink("/tmp/bsem2");
  readable = sem_open("/tmp/bsem1", O_CREAT, 0644, 0);
  writable = sem_open("/tmp/bsem2", O_CREAT, 0644, BUF_SIZE);

  printf("Creating worker threads\n");
  for (int i = 0; i < WORKER_COUNT; i++) {
    printf("Creating thread %d\n", i);
    pthread_create(&thread_ids[i], NULL, thread_func, &stupid_way_to_pass_int[i]);
  }

  // add values
  srand(time(NULL));  
  nonce = rand();

  // do validation
  uint8_t hash[32];
  struct mine_result cur;
  uint32_t sum;

  while (1) {
    sem_wait(readable);

    cur = buffer[rp];
    sum = nonce + cur.value;
    memset(hash, 0x00, sizeof hash);
    calc_sha_256(hash, (const void *)(&sum), 32U);

    _print_hash(hash);
    printf("   sum: %u\n", sum);
    printf("successful? %d\n", successful(hash));

    if (successful(hash)) {
      printf("Worker %d successfully mined %u\n", cur.worker, cur.value);
      successes[cur.worker] += 1;
      nonce = rand();
    }

    printf("rp is %d\n", rp);
    rp = (rp + 1) % BUF_SIZE;
    sem_post(writable);
  }
}
