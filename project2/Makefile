CC = gcc
CFLAGS = -Wall
#########

programs = myshell
all: $(programs)

$(objects): %: %.c
	$(CC) $(CFLAGS) -o $@ $<
