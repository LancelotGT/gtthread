#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <gtthread.h>

#include "philosopher.h"

gtthread_mutex_t chopsticks[5];
/*
 * Performs necessary initialization of mutexes.
 */
void chopsticks_init(){
    int i;
    for (i = 0; i < 5; i++)
        gtthread_mutex_init(&chopsticks[i]);
}

/*
 * Cleans up mutex resources.
 */
void chopsticks_destroy(){
    int i;
    for (i = 0; i < 5; i++)
        gtthread_mutex_destroy(&chopsticks[i]); 
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
        gtthread_mutex_lock(&chopsticks[left_chopstick]);
        pickup_left_chopstick(phil_id);
        gtthread_mutex_lock(&chopsticks[right_chopstick]);
        pickup_right_chopstick(phil_id);
    }
    else
    {
        gtthread_mutex_lock(&chopsticks[right_chopstick]);
        pickup_right_chopstick(phil_id);
        gtthread_mutex_lock(&chopsticks[left_chopstick]);
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
    if (gtthread_mutex_unlock(&chopsticks[left_chopstick]) != 0)
    {
        printf("unlock error.\n");
        exit(1);
    } 
    putdown_right_chopstick(phil_id);
    if (gtthread_mutex_unlock(&chopsticks[right_chopstick]) != 0)
    {
        printf("unlock error.\n");
        exit(1);
    }  
}
