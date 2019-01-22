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
	if (rfd < 0){
		fprintf(stderr, "%s: couldn't open %s: %s.\n", argv[0], src, strerror(errno));
		return EXIT_FAILURE;
	}
	// open dest
	int wfd = open(dest, O_RDONLY | O_CREAT, 0644);
	if (wfd < 0){
		fprintf(stderr, "%s: couldn't open %s: %s.\n", argv[0], dest, strerror(errno));
		return EXIT_FAILURE;
	}
}
