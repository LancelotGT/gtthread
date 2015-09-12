#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <pthread.h>

/* NO NEED TO MODIFY */

#define SERVINGS 100000

#include "philosopher.h"
#include "chopsticks.h"

static double const thinking_to_eating = 0.02;
static double const eating_to_thinking = 0.05;

static unsigned int seed[5];

static void eat(int phil_id){
  start_eating(phil_id);
  while ( (rand_r(&seed[phil_id]) / (RAND_MAX + 1.0)) >= eating_to_thinking);
  stop_eating(phil_id);
}

static void think(int phil_id){
  while ((rand_r(&seed[phil_id]) / (RAND_MAX + 1.0)) >= thinking_to_eating);
}

static int servings = SERVINGS;

static void* philosodine(void* arg){
  intptr_t phil_id;

  phil_id = (intptr_t) arg;

  seed[phil_id] = phil_id+1;

  while(0 < __sync_fetch_and_sub(&servings, 1L)){
    /* Philosopher was just served */

    /* Picks up his chopsticks */
    pickup_chopsticks(phil_id);

    /* Eats */
    eat(phil_id);

    /* Puts down his chopsticks */
    putdown_chopsticks(phil_id);

    /* And then thinks. */
    think(phil_id);
  }

  return NULL;
}

int main(){
  long i;
  pthread_t phil_threads[5];

  chopsticks_init();  

  for(i = 0; i < 5; i++)
    pthread_create(&phil_threads[i], NULL, philosodine, (void*) i);

  for(i = 0; i < 5; i++)
    pthread_join(phil_threads[i], NULL);    
  
  chopsticks_destroy();

  for(i = 0; i < 5; i++)
    printf("Philosopher %ld ate %d meals.\n", i, count_meals_eaten(i));

  return 0;
}
