CC = gcc
CFLAGS = -Wall
#########

programs = who tree
all: $(programs)

$(objects): %: %.c
	$(CC) $(CFLAGS) -o $@ $<
