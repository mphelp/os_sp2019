#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARACTER_INPUT 1000
#define MAX_DISTINCT_WORDS 	100

// helper functions
void printWords(char* words[]){
	for (int w = 0; words[w] != NULL; w++){
		printf("%s ",words[w]);
	}
	printf("\n");
}

int main(int argc, char* argv[]){
	// Read from line
	char line[MAX_CHARACTER_INPUT];
	char* words[MAX_DISTINCT_WORDS];

	printf("Enter message: ");

	while (fgets(line, MAX_CHARACTER_INPUT, stdin)){
		
		words[0] = strtok(line," \t\n");
		int nwords = 1;
		while ((words[nwords] = strtok(0, " \t\n")) != NULL){
			if (nwords == MAX_DISTINCT_WORDS){
				break;
			}
			nwords++;
		}
		words[nwords] = 0; 
	}
	// Print each word
	printWords(words);

	return EXIT_SUCCESS;
}
