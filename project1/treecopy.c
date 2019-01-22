#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

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
		fprintf(stderr, "%s: Unable to open dir %s: %s.\n", argv[0], srcDir, strerror(errno));
		return EXIT_FAILURE;
	}
	printf("%d\n",sd);
	// make dest dir
	int wfd = mkdir(destDir, 0777);
	if (wfd < 0){
		fprintf(stderr, "%s: Unable to make dest dir %s: %s.\n", argv[0], destDir, strerror(errno));
		return EXIT_FAILURE;
	}


	close(sd);
	return EXIT_SUCCESS;

}
