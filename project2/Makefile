CC = gcc
CFLAGS = -Wall
#########

programs = test
all: $(programs)

$(objects): %: %.c
	$(CC) $(CFLAGS) -o $@ $<
