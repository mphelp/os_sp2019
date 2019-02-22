
all: fractal fractalthread

fractal: fractal.o bitmap.o
	gcc -Wall fractal.o bitmap.o -o fractal -lpthread -lm
fractalthread: fractalthread.o bitmap.o
	gcc -Wall fractalthread.o bitmap.o -o fractalthread -lpthread -lm

fractal.o: fractal.c
	gcc --std=c99 -Wall -g -c fractal.c -o fractal.o
fractalthread.o: fractalthread.c
	gcc --std=c99 -Wall -g -c fractalthread.c -o fractalthread.o

bitmap.o: bitmap.c
	gcc --std=c99 -Wall -g -c bitmap.c -o bitmap.o

clean:
	rm -f fractal.o bitmap.o fractal
