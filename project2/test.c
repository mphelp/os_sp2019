#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARACTER_INPUT 1000
#define MAX_DISTINCT_WORDS 	100

int main(int argc, char* argv[]){
	// Read from line
	char line[MAX_CHARACTER_INPUT];
	char* words[MAX_DISTINCT_WORDS];

	printf("Enter message: ");
	strtok(line," \t\n");
	while (fgets(line, MAX_CHARACTER_INPUT, stdin)){
		
		strtok(line," \t\n");
		printf("%s\n",line);
	}

	return EXIT_SUCCESS;
}
