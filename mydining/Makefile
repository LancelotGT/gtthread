CC = gcc            # default is CC = cc
CFLAGS = -g -Wall   # default is CFLAGS = [blank]
PROJ_DIR = ..
INC_DIR = $(PROJ_DIR)/include
LIB_DIR = $(PROJ_DIR)/lib

# pattern rule for object files
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

#### Dining ####
all:dining_main 

chopsticks.o:chopsticks.c
	$(CC) $(CFLAGS) -c $^ -I$(INC_DIR) -L$(LIB_DIR) -lgtthread

philosopher.o:philosopher.c
	$(CC) $(CFLAGS) -c $^ -I$(INC_DIR) -L$(LIB_DIR) -lgtthread 

dining_main.o:dining_main.c
	$(CC) $(CFLAGS) -c $^ -I$(INC_DIR) -L$(LIB_DIR) -lgtthread  

dining_main: chopsticks.o philosopher.o dining_main.o
	$(CC) $(CFLAGS) -o dining_main $^ -I$(INC_DIR) -L$(LIB_DIR) -lgtthread

clean:
	$(RM) -f *.o dining_main
