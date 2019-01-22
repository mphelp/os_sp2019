CC = gcc
CFLAGS = -Wall
#########

programs = filecopy treecopy
all: $(programs)

$(objects): %: %.c
	$(CC) $(CFLAGS) -o $@ $<
