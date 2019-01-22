#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	if (argc != 3){
		fprintf(stderr, "Usage: %s [src] [dest]\n", argv[0]);
		return EXIT_FAILURE;
	}

	char* src = argv[1];
	char* dest = argv[2];

	// open src
	int rfd = open(src, O_RDONLY);
}
