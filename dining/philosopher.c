#include <assert.h>
#include <unistd.h>
#include "philosopher.h"

static philosopher_t philosopher[5] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
static int meals_eaten[5] = {0,0,0,0,0};

void pickup_left_chopstick(int phil_id){
	assert(!philosopher[(phil_id+4) % 5].holds_right);
  philosopher[phil_id].holds_left = 1;
}

void putdown_left_chopstick(int phil_id){
	assert(philosopher[phil_id].holds_left);
  philosopher[phil_id].holds_left = 0;
}

void pickup_right_chopstick(int phil_id){
	assert(!philosopher[(phil_id+1) % 5].holds_left);
  philosopher[phil_id].holds_right = 1;
}

void putdown_right_chopstick(int phil_id){
	assert(philosopher[phil_id].holds_right);
  philosopher[phil_id].holds_right = 0;
}

void start_eating(int phil_id){
	assert(philosopher[phil_id].holds_right);
	assert(philosopher[phil_id].holds_left);
  philosopher[phil_id].is_eating = 1;
}

void stop_eating(int phil_id){
	assert(philosopher[phil_id].holds_right);
	assert(philosopher[phil_id].holds_left);
	meals_eaten[phil_id]++;
  philosopher[phil_id].is_eating = 0;
}

int count_meals_eaten(int phil_id){
	return meals_eaten[phil_id];
}