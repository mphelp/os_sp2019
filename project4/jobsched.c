/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

/* Defines */
#define MAX_CHARACTER_INPUT 1000
#define MAX_DISTINCT_WORDS 	100
// Courtesy of Prof. Bui's Systems Programming course:
#define debug(M, ...)\
	fprintf(stderr, "%s:%d:%s" M "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define errInput(M, ...)\
	fprintf(stderr, "%s: " M "\n", __FILE__, ##__VA_ARGS__)
#define streq(s0, s1) (strcmp((s0), (s1)) == 0)


/* Command Line Parsing */
void parseWordsFromLine(char* words[], char line[]){
	words[0] = strtok(line," \t\n");
	int nwords = 1;
	while ((words[nwords] = strtok(0, " \t\n")) != NULL){
		if (nwords == MAX_DISTINCT_WORDS)
			break;
		nwords++;
	}
	words[nwords] = NULL;
}

char PROMPT[] = "> ";

int main(int argc, char* argv[]){
	while (true){
		if (fprintf(stdout, "%s", PROMPT) < 0){
			debug("failed to print prompt %s", strerror(errno));
			return EXIT_FAILURE;
		}
		if (fflush(stdout) == EOF){
			debug("failed to flush stdout %s:", strerror(errno));
			return EXIT_FAILURE;
		}	
		char line[MAX_CHARACTER_INPUT];
		char* words[MAX_DISTINCT_WORDS];
		// Parsing from prompt
		if (fgets(line, MAX_CHARACTER_INPUT, stdin) != NULL){

			parseWordsFromLine(words, line);
			//commandFunc command;

			// Retrieve program
			if (words[0] == NULL){ // spaces or enter key
				continue;
			} else if (streq(words[0], "start")){
				/* command = &startFunc; */
			} else if (streq(words[0], "run")){
				/* command = &runFunc; */
			} else if (streq(words[0], "wait")){
				/* command = &waitFunc; */
			} else if (streq(words[0], "waitfor")){
				/* command = &waitforFunc; */
			} else if (streq(words[0], "watchdog")){
				/* command = &watchdogFunc; */
			} else if (streq(words[0], "quit") || streq(words[0], "exit")){
				return EXIT_SUCCESS;
			} else {
				fprintf(stderr, "... Command \'%s\' not recognized\n", words[0]);
				continue;
			}
			// Call command:
			/* int commandReturn = (*command)(words); */
			/* if (commandReturn == EXIT_FAILURE){ */
				// Optional space for general command failure error checking
			/* } */
		} else {
			// Exit prompt
			printf("\n");
			return EXIT_SUCCESS;
		}
	}
}
