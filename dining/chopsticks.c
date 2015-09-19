#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <pthread.h>

#include "philosopher.h"

static pthread_mutex_t chopsticks[5];
/*
 * Performs necessary initialization of mutexes.
 */
void chopsticks_init(){
    int i;
    for (i = 0; i < 5; i++)
        pthread_mutex_init(&chopsticks[i], NULL);
}

/*
 * Cleans up mutex resources.
 */
void chopsticks_destroy(){
    int i;
    for (i = 0; i < 5; i++)
        pthread_mutex_destroy(&chopsticks[i]); 
}

/*
 * Uses pickup_left_chopstick and pickup_right_chopstick
 * to pick up the chopsticks
 */   
void pickup_chopsticks(int phil_id){
    /* simple solution, always pick up the chopstick with lower number */
    int left_chopstick = (phil_id + 4) % 5;
    int right_chopstick = phil_id;
    if (left_chopstick < right_chopstick)
    {
        pthread_mutex_lock(&chopsticks[left_chopstick]);
        pickup_left_chopstick(phil_id);
        pthread_mutex_lock(&chopsticks[right_chopstick]);
        pickup_right_chopstick(phil_id);
    }
    else
    {
        pthread_mutex_lock(&chopsticks[right_chopstick]);
        pickup_right_chopstick(phil_id);
        pthread_mutex_lock(&chopsticks[left_chopstick]);
        pickup_left_chopstick(phil_id); 
    }
}

/*
 * Uses pickup_left_chopstick and pickup_right_chopstick
 * to pick up the chopsticks
 */   
void putdown_chopsticks(int phil_id){
    int left_chopstick = (phil_id + 4) % 5;
    int right_chopstick = phil_id; 
    putdown_left_chopstick(phil_id);
    pthread_mutex_unlock(&chopsticks[left_chopstick]); 
    putdown_right_chopstick(phil_id);
    pthread_mutex_unlock(&chopsticks[right_chopstick]);  
}
