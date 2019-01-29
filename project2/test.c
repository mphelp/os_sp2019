#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARACTER_INPUT 1000
#define MAX_DISTINCT_WORDS 	100
#define BUF_CUSTOM 					10

int main(int argc, char* argv[]){
	// Read from line
	char buf[BUF_CUSTOM];
	char* text = calloc(1,1);
	printf("Enter message: ");
	while (fgets(buf, BUF_CUSTOM, stdin)){
		text = realloc(text, strlen(text)+strlen(buf)+1);
		
		strcat(text, buf);
		printf("%s\n",buf);
	}
	printf("\nAll text:%s",text);

	return EXIT_SUCCESS;
}
