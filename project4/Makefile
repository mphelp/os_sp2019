HEADERS= macros.h commands.h JobStructures.h
LIB= -lpthread

default: jobsched

jobsched: jobsched.c $(HEADERS)
	gcc jobsched.c -o $@ $(LIB)

clean:
	-rm -f $(OBJECTS)
	-rm -f jobsched
