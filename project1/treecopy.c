#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define eqstr(s1, s2) (strcmp((s1),(s2)) == 0)

/* Error msgs */
char PROGRAM[] = "";
void err(char* message, const char* file){
	fprintf(stderr, "%s: %s: %s.\n", PROGRAM, message, file);
}

int fileCopy(char* src, char* dest){
	// open src/dest
	int rfd = open(src, O_RDONLY);
	if (rfd < 0){
		err("Couldn't open src", src);
		return EXIT_FAILURE;
	}
	int wfd = open(dest, O_WRONLY | O_CREAT);
	if (wfd < 0){
		err("Couldn't open dest", dest);
		return EXIT_FAILURE;
	}

	// filecopy
	char buf[BUFSIZ];
	int nread, nwritten;
	
	while ((nread = read(rfd, buf, BUFSIZ)) != 0){
		if (nread < 0){
			err("Failed reading src", src);
			return EXIT_FAILURE;
		}
		nwritten = write(wfd, buf, nread);
		if (nwritten < 0){
			err("FAILED writing to dest", dest);
			/* fprintf(stderr, "Read %d written %d\n",nread,nwritten); */
			return EXIT_FAILURE;
		}
		// finish writes that take more than a chunk
		int nextwrites;
		while (nwritten != nread){
			if ((nextwrites = write(wfd, buf + nwritten, nread - nwritten)) < 0){
				err("FAILED writing to dest", dest);
				return EXIT_FAILURE;
			}
			nwritten += nextwrites;
		}
	}

	// finish
	close(wfd);
	close(rfd);
	printf("copied %d bytes from %s to %s\n", nwritten, src, dest);
	return EXIT_SUCCESS;
}
int walkDir(const char* root){
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
			walkDir(path);
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


	// walk dir and copy src dir
	if (walkDir(srcDir) < 0){
		return EXIT_FAILURE;
	}


	/* Cleanup */
	close(wfd);
	return EXIT_SUCCESS;
}

// Author: 	Matthew Phelps
// Date: 		Jan 22 2019
