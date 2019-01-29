#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARACTER_INPUT 1000
#define MAX_DISTINCT_WORDS 	100

int main(int argc, char* argv[]){
	// Read from line
	char buf[MAX_CHARACTER_INPUT];
	char* words[MAX_DISTINCT_WORDS];

	//char* text = calloc(1,1);
	printf("Enter message: ");
	while (fgets(buf, MAX_CHARACTER_INPUT, stdin)){
		//text = realloc(text, strlen(text)+strlen(buf)+1);
		
		//strcat(text, buf);
		printf("%s\n",buf);
	}

	return EXIT_SUCCESS;
}
