HEADERS= macros.h commands.h
OBJECTS= jobsched.o

default: jobsched

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

jobsched: $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f jobsched
