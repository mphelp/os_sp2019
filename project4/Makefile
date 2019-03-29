HEADERS= macros.h commands.h JobStructures.h
FLAGS= -Wall -g
LIB= -lpthread
OUTPUTDIR=outputs/

default: jobsched

jobsched: jobsched.c $(HEADERS)
	gcc jobsched.c $(FLAGS) -o $@ $(LIB)

clean:
	-rm -f jobsched
