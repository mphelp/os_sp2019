#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

/* Error msgs */
void err(char* message, char* file, char* program){
	fprintf(stderr, "%s: %s: %s.\n", program, message, file);
}

int main(int argc, char* argv[]){
	// arg parsing
	if (argc != 3){
		fprintf(stderr, "Incorrect # of arguments ... Usage: %s [src] [dest]\n", argv[0]);
		return EXIT_FAILURE;
	}
	char* srcDir = argv[1];
	char* destDir = argv[2];

	/* Setup */
	// open src dir
	DIR* sdir = opendir(srcDir);
	if (sdir == NULL){
		/* fprintf(stderr, "%s: Unable to open dir %s: %s.\n", argv[0], srcDir, strerror(errno)); */
		err("Unable to open dir", srcDir, argv[0]);
		return EXIT_FAILURE;
	}
	// make dest dir
	int wfd = mkdir(destDir, 0777);
	if (wfd < 0){
		fprintf(stderr, "%s: Unable to make dest dir %s: %s.\n", argv[0], destDir, strerror(errno));
		return EXIT_FAILURE;
	}

	/* Cleanup */
	if (closedir(sdir) < 0){
		fprintf(stderr,"%s: Couldn't close src dir %s: %s.\n", argv[0], destDir, strerror(errno));
		return EXIT_FAILURE;
	}
	close(wfd);
	return EXIT_SUCCESS;
}
