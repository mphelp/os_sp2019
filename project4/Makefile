HEADERS= macros.h commands.h JobStructures.h
FLAGS= -Wall -g -std=gnu99
LIB= -lpthread
OUTPUTDIR=outputs/

default: jobsched

jobsched: jobsched.c $(HEADERS)
	gcc jobsched.c $(FLAGS) -o $@ $(LIB)

clean:
	-rm -f jobsched
	-rm -rf $(OUTPUTDIR)
