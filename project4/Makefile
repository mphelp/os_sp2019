HEADERS= macros.h commands.h JobStructures.h
FLAGS= -Wall -g
LIB= -lpthread

default: jobsched

jobsched: jobsched.c $(HEADERS)
	gcc jobsched.c $(FLAGS) -o $@ $(LIB)

clean:
	-rm -f $(OBJECTS)
	-rm -f jobsched
