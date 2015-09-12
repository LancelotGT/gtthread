#ifndef _PHILOSOPHER_H_
#define _PHILOSOPHER_H_

typedef struct{
  int holds_left;
  int holds_right;
  int is_eating;
} philosopher_t;

void pickup_left_chopstick(int phil_id);

void putdown_left_chopstick(int phil_id);

void pickup_right_chopstick(int phil_id);

void putdown_right_chopstick(int phil_id);

void start_eating(int phil_id);

void stop_eating(int phil_id);

int count_meals_eaten(int phil_id);

#endif