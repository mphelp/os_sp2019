
all: fractalthread fractaltask

fractalthread: fractalthread.o bitmap.o
	gcc -Wall fractalthread.o bitmap.o -o fractalthread -lpthread -lm
fractaltask: fractaltask.o bitmap.o
	gcc -Wall fractaltask.o bitmap.o -o fractaltask -lpthread -lm

fractalthread.o: fractalthread.c
	gcc --std=c99 -Wall -g -c fractalthread.c -o fractalthread.o
fractaltask.o: fractaltask.c
	gcc --std=c99 -Wall -g -c fractaltask.c -o fractaltask.o

bitmap.o: bitmap.c
	gcc --std=c99 -Wall -g -c bitmap.c -o bitmap.o

clean:
	rm -f fractalthread.o bitmap.o fractalthread fractaltask fractaltask.o
