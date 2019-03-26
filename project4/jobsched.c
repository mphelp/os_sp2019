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

/* Commands */

/*
	 submit <command>
	 status
	 wait <jobid>
	 remove <jobid>
	 njobs <n>
	 drain
	 quit
	 help
*/


int helpFunc(char* words[]){
	printf("Usage:\n");
	printf("submit <command> - submit new job to be scheduled and run\n");
	printf("status           - get status of all jobs\n");	
	printf("wait <jobid>     - pause other jobs until chosen job finishes\n");
	printf("remove <jobid>   - removes job from job queue if waited for or done\n");
	printf("njobs <n>        - set number of jobs the scheduler can run at once\n");
	printf("drain            - wait until all jobs are in done state\n");
	printf("quit             - exit the program, regardless of jobs scheduled\n");
	printf("help             - display usage\n");
	return EXIT_SUCCESS;
}
int submitFunc(char* words[]){

	return EXIT_SUCCESS;
}
int statusFunc(char* words[]){

	return EXIT_SUCCESS;
}
int waitFunc(char* words[]){

	return EXIT_SUCCESS;
}
int removeFunc(char* words[]){

	return EXIT_SUCCESS;
}
int njobsFunc(char* words[]){

	return EXIT_SUCCESS;
}
int drainFunc(char* words[]){

	return EXIT_SUCCESS;
}

/* Command Functions */
typedef int (*commandFunc)(char**);

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
			commandFunc command;

			// Retrieve program
			// Probbaly could do for loop for specific commands and their commandFuncs
			if (words[0] == NULL){ // spaces or enter key
				continue;
			} else if (streq(words[0], "help")){
				command = &helpFunc;
			} else if (streq(words[0], "submit")){
				command = &submitFunc;
			} else if (streq(words[0], "status")){
				command = &statusFunc;
			} else if (streq(words[0], "wait")){
				command = &waitFunc;
			} else if (streq(words[0], "remove")){
				command = &removeFunc;
			} else if (streq(words[0], "njobs")){
				command = &njobsFunc;
			} else if (streq(words[0], "drain")){
				command = &drainFunc;
			} else if (streq(words[0], "quit")){
				return EXIT_SUCCESS;
			} else {
				fprintf(stderr, "... Command \'%s\' not recognized\n", words[0]);
				continue;
			}
			// Call command:
			int commandReturn = (*command)(words);
			if (commandReturn == EXIT_FAILURE){
				// Optional space for general command failure error checking
			}
		} else {
			// Exit prompt
			printf("\n");
			return EXIT_SUCCESS;
		}
	}
}
