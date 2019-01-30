#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define MAX_CHARACTER_INPUT 1000
#define MAX_DISTINCT_WORDS 	100

// errors
#define debug(M, ...)\
	fprintf(stderr, "%s: " M "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define userError(M, ...)\
	fprintf(stderr, "%s: " M "\n", __FILE__, ##__VA_ARGS__)

// helper functions
void printWords(char* words[]){
	for (int w = 0; words[w] != NULL; w++){
		printf("%s ",words[w]);
	}
	printf("\n");
}
void parseWordsFromLine(char* words[], char line[]){
	words[0] = strtok(line," \t\n");
	int nwords = 1;
	while ((words[nwords] = strtok(0, " \t\n")) != NULL){
		if (nwords == MAX_DISTINCT_WORDS)
			break;
		nwords++;
	}
	words[nwords] = 0; // null terminated
}

int main(int argc, char* argv[]){
	// Read from line
	char line[MAX_CHARACTER_INPUT];
	char* words[MAX_DISTINCT_WORDS];

	if (fprintf(stdout, "myshell> ") < 0){
		debug("failed to print prompt %s", strerror(errno));
		return EXIT_FAILURE;
	}
	if (fflush(stdout) == EOF){
		debug("failed to flush stdout %s:", strerror(errno));
		return EXIT_FAILURE;
	}
	while (true){
		char* line = fgets(line, MAX_CHARACTER_INPUT, stdin);
		if (line == NULL){
			userError("reached EOF");
			return EXIT_FAILURE;
		} else {
			parseWordsFromLine(words, line);
		}
	}
	printWords(words);	

	return EXIT_SUCCESS;
}
