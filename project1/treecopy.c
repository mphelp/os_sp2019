#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

/* Error msgs */
char PROGRAM[] = "";
void err(char* message, char* file){
	fprintf(stderr, "%s: %s: %s.\n", PROGRAM, message, file);
}

int main(int argc, char* argv[]){
	// arg parsing
	strcpy(PROGRAM, argv[0]);
	if (argc != 3){
		fprintf(stderr, "Incorrect # of args ... Usage: %s [src] [dest]\n", PROGRAM);
		return EXIT_FAILURE;
	}
	char* srcDir = argv[1];
	char* destDir = argv[2];

	/* Setup */
	// open src dir
	DIR* sdir = opendir(srcDir);
	if (sdir == NULL){
		err("Unable to open dir", srcDir);
		return EXIT_FAILURE;
	}
	// make dest dir
	int wfd = mkdir(destDir, 0777);
	if (wfd < 0){
		err("Unable to make dest dir", destDir);
		return EXIT_FAILURE;
	}


	/* Walking src dir */





	/* Cleanup */
	if (closedir(sdir) < 0){
		err("Couldn't close src dir", srcDir);
		return EXIT_FAILURE;
	}
	close(wfd);
	return EXIT_SUCCESS;
}
