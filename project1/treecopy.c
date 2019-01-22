#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define eqstr(s1, s2) (strcmp((s1),(s2)) == 0)

/* Error msgs */
char PROGRAM[] = "";
void err(char* message, const char* file){
	fprintf(stderr, "%s: %s: %s.\n", PROGRAM, message, file);
}

int walk(const char* root){
	// open root
	DIR* dir = opendir(root);
	if (dir == NULL){
		err("Unable to open dir", root);
		return EXIT_FAILURE;
	}
	// Walking src dir
	for (struct dirent* e = readdir(dir); e; e = readdir(dir)){
		if (eqstr(e->d_name, "..") || eqstr(e->d_name, "."))
			continue;
		char path[BUFSIZ];

		snprintf(path, BUFSIZ, "%s/%s", root, e->d_name);
		puts(path);

		// HERE COPY

		// recurse
		if (e->d_type == DT_DIR){
			walk(path);
		}	
	}
	// finish by closing
	if (closedir(dir) < 0){
		err("Couldn't close src dir", root);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

// MAIN
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
	
	// make dest dir
	int wfd = mkdir(destDir, 0777);
	if (wfd < 0){
		err("Unable to make dest dir", destDir);
		return EXIT_FAILURE;
	}


	// walk and copy src dir
	if (walk(srcDir) < 0){
		return EXIT_FAILURE;
	}


	/* Cleanup */
	close(wfd);
	return EXIT_SUCCESS;
}

// Author: 	Matthew Phelps
// Date: 		Jan 22 2019
