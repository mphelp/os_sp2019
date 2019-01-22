#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	// arg parsing
	if (argc != 3){
		fprintf(stderr, "Incorrect # of arguments ... Usage: %s [src] [dest]\n", argv[0]);
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
	int wfd = open(dest, O_WRONLY | O_CREAT);
	if (wfd < 0){
		fprintf(stderr, "%s: couldn't open %s: %s.\n", argv[0], dest, strerror(errno));
		return EXIT_FAILURE;
	}

	// filecopy from src to dest
	char buf[BUFSIZ];
	int nread, nwritten;

	while ((nread = read(rfd, buf, BUFSIZ)) != 0){
		if (nread < 0){
			fprintf(stderr, "%s: failed reading %s: %s.\n", argv[0], src, strerror(errno));
			return EXIT_FAILURE;
		}
		nwritten = write(wfd, buf, nread);
		if (nwritten < 0){
			fprintf(stderr, "%s: FAILED writing to %s: %s.\n", argv[0], dest, strerror(errno));
			fprintf(stderr, "Read %d written %d\n",nread,nwritten);
			return EXIT_FAILURE;
		}
		// finish writes that take more than a chunk
		int nextwrites;
		while (nwritten != nread){
			if ((nextwrites = write(wfd, buf + nwritten, nread - nwritten)) < 0){
				fprintf(stderr, "%s: failed writing to %s: %s.\n", argv[0], dest, strerror(errno));
				return EXIT_FAILURE;
			}
			nwritten += nextwrites;
		}
	}

	// finish
	close(wfd);
	close(rfd);
	printf("%s: copied %d bytes from %s to %s\n", argv[0], nwritten, src, dest);
	return EXIT_SUCCESS;
}
