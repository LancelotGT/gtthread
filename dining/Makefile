CC = gcc            # default is CC = cc
CFLAGS = -g -Wall   # default is CFLAGS = [blank]

# pattern rule for object files
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

#### Dining ####
dining_main: dining_main.o chopsticks.o philosopher.o
	$(CC) -o dining_main $^ -lpthread

clean:
	$(RM) -f *.o dining_main
