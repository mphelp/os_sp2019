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

// Global copy information struct
struct copyInfo {
	int files, directories, bytes;
};
struct copyInfo COPYINFO = { 0,0,0 };

/* Error msgs */
char PROGRAM[] = "";
void err(char* message, const char* file){
	fprintf(stderr, "%s: %s: %s.\n", PROGRAM, message, file);
}

// Copy functions
int fileCopy(char* src, char* dest){
	// open src/dest
	int rfd = open(src, O_RDONLY);
	if (rfd < 0){
		err("Couldn't open src", src);
		return EXIT_FAILURE;
	}
	int wfd = open(dest, O_WRONLY | O_CREAT, 0666); 
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
			err("Failed writing to dest", dest);
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

	// add copy information
	COPYINFO.bytes += nwritten;
	COPYINFO.files += 1;

	// finish
	close(wfd);
	close(rfd);
	return EXIT_SUCCESS;
}
int dirCopy(const char* root, const char* newRoot){
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
		char newPath[BUFSIZ];

		snprintf(path, BUFSIZ, "%s/%s", root, e->d_name);
		snprintf(newPath, BUFSIZ, "%s/%s", newRoot, e->d_name);

		// print copy
		printf("%s -> %s\n", path, newPath);

		// recurse on directory
		if (e->d_type == DT_DIR){ // directory
			int wfd = mkdir(newPath, 0777);
			if (wfd < 0){
				err("Unable to make dest dir", newPath);
				return EXIT_FAILURE;
			}
			dirCopy(path, newPath);
		}	else if (e->d_type == DT_REG){ // standard file
			fileCopy(path, newPath);
		} else { // anything else
			err("Unable to process file", newPath);
			return EXIT_FAILURE;
		}
	}
	// add copy information
	COPYINFO.directories += 1;
	
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
		fprintf(stderr, "usage: %s <sourcefile> <targetfile>\n", PROGRAM);
		return EXIT_FAILURE;
	}
	char* srcDir = argv[1];
	char* destDir = argv[2];

	// make dest dir (check if already exists)
	struct stat buffer;
	if (stat(destDir, &buffer) == 0){
		err("Destination directory already exists", destDir);
		return EXIT_FAILURE;
	}
	int wfd = mkdir(destDir, 0777);
	if (wfd < 0){
		err("Unable to make dest dir", destDir);
		return EXIT_FAILURE;
	}
	// print first copy (src dir to dest dir)
	printf("%s -> %s\n", srcDir, destDir);

	// walk and copy src dir
	if (dirCopy(srcDir, destDir) < 0){
		return EXIT_FAILURE;
	}
	// Display copy information
	printf("%s: copied %d directories, %d files, ", PROGRAM, COPYINFO.directories, COPYINFO.files);
	printf("and %d bytes from %s to %s\n", COPYINFO.bytes, srcDir, destDir);

	/* Cleanup */
	close(wfd);
	return EXIT_SUCCESS;
}

// Author: 	Matthew Phelps
// Date: 		Jan 22 2019
