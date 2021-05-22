#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sha-256.h"

#define DIFFICULTY 2 // number of bytes of leading 0's
#define WORKER_COUNT 4 // number of worker threads
#define BUF_SIZE 16 // number of entries in ring buffer

uint32_t nonce, total;

//
// thread stuff
//

pthread_t thread_ids[WORKER_COUNT];
int successes[WORKER_COUNT] = {0};

// how do you do this the right way? this is so so so dumb
int stupid_way_to_pass_int[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

//
// ring buffer stuff
//

struct mine_result {
  uint32_t value;
  uint32_t target;
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


void *thread_func(void *id) {
  int worker = *(int *) id;

  uint8_t hash[32];
  uint32_t x, sum;

  // do the work
  while (1) {
    do {
      x = rand();
      sum = nonce + x;
      calc_sha_256(hash, (const void *)(&sum), 4);
    } while (!successful(hash));

    struct mine_result to_write = {.value=x, .target=nonce, .worker=worker};
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
  sem_unlink("/tmp/bsem1");
  sem_unlink("/tmp/bsem2");
  readable = sem_open("/tmp/bsem1", O_CREAT, 0644, 0);
  writable = sem_open("/tmp/bsem2", O_CREAT, 0644, BUF_SIZE);

  printf("Creating worker threads\n");
  for (int i = 0; i < WORKER_COUNT; i++) {
    printf("Creating thread %d\n", i);
    pthread_create(&thread_ids[i], NULL, thread_func, &stupid_way_to_pass_int[i]);
  }

  srand(time(NULL));  
  nonce = rand();

  uint8_t hash[32];
  struct mine_result cur;
  uint32_t sum, prev;

  while (1) {
    sem_wait(readable);

    // do validation
    cur = buffer[rp];
    sum = nonce + cur.value;
    calc_sha_256(hash, (const void *)(&sum), 4);

    if (successful(hash)) {
      successes[cur.worker] += 1;
      total += 1;

      if (total % 50 == 0) {
        printf(
          "leaderboard (total %d):\n0: %d\n1: %d\n2: %d\n3: %d\n",
          total,
          successes[0],
          successes[1],
          successes[2],
          successes[3]
        );
      }

      prev = nonce;
      nonce = rand();
    } else {
      // log unsuccessful attempts, see why failed
      printf("FAIL target: %u / nonce: %u / prev: %u\n", cur.target, nonce, prev);
    }

    rp = (rp + 1) % BUF_SIZE;

    sem_post(writable);
  }
}
